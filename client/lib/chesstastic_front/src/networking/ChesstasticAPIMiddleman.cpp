//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChesstasticAPIMiddleman.h"

void ChesstasticAPIMiddleman::poll() {
    while (true) {
        auto future = this->apiservice.get()->poll();
        future.wait();
        auto variant = future.get();
        if (auto poll_res = std::get_if<ChesstasticNetworking::EndpointPollReply>(&variant)) {
            std::cout << poll_res->action << std::endl;
            std::cout << "[POLL] Server action: " << poll_res->action << std::endl;
            if (poll_res->action != "idle") {
                emit new_game(poll_res->game.value());
                break;
            }
        } else {
            auto error = std::get_if<ChesstasticNetworking::EndpointErrorReply>(&variant);
            std::cout << "[POLL] Error: " << error->code << " -> " << error->description << std::endl;
        }
    }
}
