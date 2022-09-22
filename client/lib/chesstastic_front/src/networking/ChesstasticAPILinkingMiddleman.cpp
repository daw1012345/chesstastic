//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChesstasticAPILinkingMiddleman.h"
#include "NetworkingEndpointDataModels.h"

ChesstasticAPILinkingMiddleman::ChesstasticAPILinkingMiddleman(
        std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> service)
        : apiService(service) {
}

std::string ChesstasticAPILinkingMiddleman::start() {
    auto token = this->random_token();
    std::thread([=]() {
        run(token);
    }).detach();

    return token;
}

void ChesstasticAPILinkingMiddleman::run(std::string token) {
    while (true) {
        auto future = this->apiService.get()->link(token);
        future.wait();

        auto variant = future.get();
        if (auto linkData = std::get_if<ChesstasticNetworking::EndpointLinkReply>(&variant)) {
            this->apiService->set_auth(linkData->uid, linkData->token);
            auto future_info = this->apiService.get()->info();
            future_info.wait();
            auto res = future_info.get();
            if (auto user_data = std::get_if<ChesstasticNetworking::EndpointInfoReply>(&res)) {
                emit linked(QString::fromStdString(user_data->username));
                break;
            }
        }
    }
}

std::string ChesstasticAPILinkingMiddleman::random_token_from_file(std::ifstream& fd, int n) {
// This is not the optimal solution, however it is ran once per execution and the memory is freed on exit
// TODO: Use reservoir sampling

    std::string cur_line;
    std::vector<std::string> words;
    std::random_device r;
    std::default_random_engine e1(r());

    while(std::getline(fd, cur_line)) {
        words.push_back(cur_line);
    }

    std::uniform_int_distribution<int> uniform_dist(0, words.size());
    std::vector<std::string> token_components;

    while(token_components.size() < n) {
        int random_word_idx = uniform_dist(e1);

        auto word = words.at(random_word_idx);
        if (word.length() >= MIN_BLOCK_LEN) {
            token_components.push_back(word);
        }
    }

    return std::accumulate(token_components.begin(), token_components.end(), std::string{},
                    [](std::string acc, std::string lhs)
                    {
                        return acc.empty() ? lhs : acc + "-" + lhs;
                    });
}


std::string ChesstasticAPILinkingMiddleman::random_token() {
    std::string candidate_path = "/usr/share/dict/words";
    std::ifstream fd(candidate_path);

    if (fd) {
        return random_token_from_file(fd, TOKEN_BLOCK_NUM);
    } else {
        return "CAN'T-GENERATE-TOKEN";
    }
}
