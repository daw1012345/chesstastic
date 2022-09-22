//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "NetworkingEndpointDataModels.h"

namespace ChesstasticNetworking {
    void from_json(const nlohmann::json &j, EndpointGameState &p) {
        if (j["piece"].is_null()) {
            p.piece = std::nullopt;
        } else {
            p.piece = j.at("piece").get<EndpointGamePiece>();
        }

        j.at("position").get_to(p.position);

    }

    void to_json(nlohmann::json &j, const EndpointGameState &p) {
        j = nlohmann::json{{"position", p.position}};
        j["piece"] = p.piece.has_value() ? nlohmann::json(p.piece.value()) : nlohmann::json();
    }

    void from_json(const nlohmann::json &j, EndpointGameReply &p) {
        j.at("white").get_to(p.white);
        j.at("black").get_to(p.black);
        j.at("board").get_to(p.board);
        j.at("game_over").get_to(p.game_over);

        if (j["winner"].is_null()) {
            p.winner = std::nullopt;
        } else {
            p.winner = j.at("winner").get<std::string>();
        }

    }

    void to_json(nlohmann::json &j, const EndpointGameReply &p) {
        j = nlohmann::json{{"white",    p.white},
                           {"black", p.black},
                           {"board", p.board},
                           {"game_over", p.game_over}};

        j["winner"] =  p.winner.has_value() ? nlohmann::json(p.winner.value()) : nlohmann::json();
    }

    void from_json(const nlohmann::json &j, EndpointPollReply &p) {
        j.at("action").get_to(p.action);
        if (j["game"].is_null()) {
            p.game = std::nullopt;
        } else {
            p.game = j.at("game").get<int>();
        }
    }

    void to_json(nlohmann::json &j, const EndpointPollReply &p) {
        j = nlohmann::json{{"action", p.action}};

        j["game"] = p.game.has_value() ? nlohmann::json(p.game.value()) : nlohmann::json();
    }

}
