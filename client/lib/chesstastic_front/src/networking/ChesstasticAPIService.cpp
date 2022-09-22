//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChesstasticAPIService.h"
#include <future>
#include <list>
#include <variant>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>

namespace ChesstasticNetworking {
    ChesstasticAPIService::ChesstasticAPIService(std::string uid, std::string token) {
        this->uid = uid;
        this->token = token;
    }

    ChesstasticAPIService::ChesstasticAPIService() {
        QString suid = settings.value("uid").toString();
        QString stoken = settings.value("token").toString();

        if (!suid.isEmpty() && !stoken.isEmpty()) {
            uid = suid.toStdString();
            token = stoken.toStdString();
        }
    }

    template<typename T>
    std::variant<T, EndpointErrorReply, LocalAPIError> ChesstasticAPIService::parse(httplib::Result &res, T type) const {
        try {
            if (res->status == 200 && res.error() == httplib::Error::Success) {
                nlohmann::json j = nlohmann::json::parse(res->body);
                return j.get<T>();
            } else if (res->status != 200 && res.error() == httplib::Error::Success) {
                nlohmann::json j = nlohmann::json::parse(res->body);
                return j["error"].get<EndpointErrorReply>();
            } else {
                return LocalAPIError{"Client Error", res->reason, true};
            }
        } catch (nlohmann::json::parse_error json_error) {
            return LocalAPIError{"Invalid Reply", json_error.what(), true};
        }
    }


    template<typename T>
    std::future<std::variant<T, EndpointErrorReply, LocalAPIError>>
    ChesstasticAPIService::request(const std::string url, const std::string body, std::string token, T type,
                                   bool is_post) const {
        return std::async(std::launch::async, [=]() {
            httplib::Client cli(SERVER);
            cli.set_follow_location(true);
            httplib::Headers headers;
            if (!token.empty()) {
                headers.emplace("Authorization", "Bearer " + token);
            }

            if (!is_post) {
                auto res = cli.Get(url.c_str(), headers);
                return parse(res, type);
            } else {
                headers.emplace("Content-Type", "application/json");
                auto res = cli.Post(url.c_str(), headers, body, "application/json");
                return parse(res, type);
            }
        });
    }

    LinkResult
    ChesstasticAPIService::link(std::string token) const {
        return request(API_LINK, "", token, EndpointLinkReply(), true);
    }

    InfoResult ChesstasticAPIService::info() const {
        auto token =  make_token(this->token);
        if (!token.has_value()) {
            std::promise<InfoVariant> promise;
            auto future = promise.get_future();

            promise.set_value(LocalAPIError{"Invalid Token", "Can't authenticate with server", false});
            return future;
        }

        return request(API_INFO, "", this->uid + ":" + token.value(), EndpointInfoReply(), false);
    }

    MoveResult
    ChesstasticAPIService::move(int game, EndpointGameRequest move) const {
        nlohmann::json j;
        j["move"] = move;

        auto token =  make_token(this->token);
        if (!token.has_value()) {
            std::promise<MoveVariant> promise;
            auto future = promise.get_future();

            promise.set_value(LocalAPIError{"Invalid Token", "Can't authenticate with server", false});
            return future;
        }

        return request(API_GAME + std::to_string(game) + "/", j.dump(), this->uid + ":" + token.value(),
                       EndpointGameReply(), true);
    }

    GameResult ChesstasticAPIService::game(int game) const {
        auto token =  make_token(this->token);
        if (!token.has_value()) {
            std::promise<GameVariant> promise;
            auto future = promise.get_future();

            promise.set_value(LocalAPIError{"Invalid Token", "Can't authenticate with server", false});
            return future;
        }

        return request(API_GAME + std::to_string(game), "", this->uid + ":" + token.value(),
                       EndpointGameReply(),
                       false);
    }

    PollResult ChesstasticAPIService::poll() const {
        auto token =  make_token(this->token);
        if (!token.has_value()) {
            std::promise<PollVariant> promise;
            auto future = promise.get_future();

            promise.set_value(LocalAPIError{"Invalid Token", "Can't authenticate with server", false});
            return future;
        }

        return request(API_POLL, "", this->uid + ":" + token.value(), EndpointPollReply(), false);
    }

    std::optional<std::string> ChesstasticAPIService::make_token(std::string base) const {
        std::stringstream time_buffer;

        std::time_t time = std::time(nullptr);
        std::tm ltim = *std::localtime(&time);

        time_buffer << std::put_time(&ltim, "%Y%m%d%H");
        std::string raw_token = base + time_buffer.str();

        int res = 1;

        unsigned char hash[SHA256_DIGEST_LENGTH]; // TODO: Check ret
        SHA256_CTX sha256;
        res &= SHA256_Init(&sha256);
        res &= SHA256_Update(&sha256, raw_token.c_str(), raw_token.length());
        res &= SHA256_Final(hash, &sha256);

        if (!res) {
            return std::nullopt;
        }

        std::stringstream sha_stream;

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sha_stream << std::setw(2) << std::setfill('0') << std::hex << (int) hash[i];
        }

        return sha_stream.str();
    }

    void ChesstasticAPIService::set_auth(std::string uid, std::string token) {
        settings.setValue("uid", QString::fromStdString(uid));
        settings.setValue("token", QString::fromStdString(token));

        this->uid = uid;
        this->token = token;
    }

    void ChesstasticAPIService::reset_auth() {
        settings.remove("uid");
        settings.remove("token");

        this->uid = "";
        this->token = "";
    }

}