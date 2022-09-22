//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICAPISERVICE_H
#define CHESSTASTIC_CHESSTASTICAPISERVICE_H

#include <string>
#include "NetworkingEndpointDataModels.h"
#include <future>
#include <httplib.h>
#include <variant>
#include <QSettings>

namespace ChesstasticNetworking {
    typedef std::variant<EndpointLinkReply, EndpointErrorReply, LocalAPIError> LinkVariant;
    typedef std::future<LinkVariant> LinkResult;
    typedef std::variant<EndpointInfoReply, EndpointErrorReply, LocalAPIError> InfoVariant;
    typedef std::future<InfoVariant> InfoResult;
    typedef std::variant<EndpointPollReply, EndpointErrorReply, LocalAPIError> PollVariant;
    typedef std::future<PollVariant> PollResult;
    typedef std::variant<EndpointGameReply, EndpointErrorReply, LocalAPIError> GameVariant;
    typedef std::future<GameVariant> GameResult;
    typedef std::variant<EndpointGameReply, EndpointErrorReply, LocalAPIError> MoveVariant;
    typedef std::future<MoveVariant> MoveResult;

    class ChesstasticAPIService {
    public:
        ChesstasticAPIService(std::string, std::string);

        ChesstasticAPIService();

        LinkResult link(std::string) const;

        InfoResult info() const;

        PollResult poll() const;

        GameResult game(int) const;

        MoveResult move(int, EndpointGameRequest) const;

        void set_auth(std::string, std::string);

        void reset_auth();

        bool is_linked() {
            return !uid.empty() && !token.empty();
        }

    private:
        std::string uid;
        std::string token;
        QSettings settings;

        template<typename T>
        std::future<std::variant<T, EndpointErrorReply, LocalAPIError>>
        request(std::string, std::string, std::string, T, bool = false) const;

        template<typename T>
        inline std::variant<T, EndpointErrorReply, LocalAPIError> parse(httplib::Result &, T) const;

        std::optional<std::string> make_token(std::string) const;

        const std::string SERVER = "https://";
        const std::string API_BASE = "/api/";
        const std::string API_LINK = API_BASE + "link/";
        const std::string API_INFO = API_BASE + "info/";
        const std::string API_POLL = API_BASE + "poll/";
        const std::string API_GAME = API_BASE + "game/";
    };
}

#endif //CHESSTASTIC_CHESSTASTICAPISERVICE_H
