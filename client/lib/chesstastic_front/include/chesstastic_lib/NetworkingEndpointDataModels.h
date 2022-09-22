//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_NETWORKINGENDPOINTDATAMODELS_H
#define CHESSTASTIC_NETWORKINGENDPOINTDATAMODELS_H

#include <nlohmann/json.hpp>

namespace ChesstasticNetworking {
    struct LocalAPIError {
        std::string title;
        std::string description;
        bool is_recoverable;
    };

    struct EndpointErrorReply {
        std::string code;
        std::string description;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointErrorReply, code, description);

    struct EndpointLinkReply {
        std::string uid;
        std::string token;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointLinkReply, uid, token);

    struct EndpointInfoReply {
        std::string username;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointInfoReply, username);

    struct EndpointPollReply {
        std::string action;
        std::optional<int> game;
    };

    void from_json(const nlohmann::json &j, EndpointPollReply &p);

    void to_json(nlohmann::json &j, const EndpointPollReply &p);


    struct EndpointGameCoordinates {
        int row;
        int col;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGameCoordinates, row, col);

    struct EndpointGamePosition {
        int index;
        EndpointGameCoordinates coordinates;
        std::string algebraic;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGamePosition, index, coordinates, algebraic);

    struct EndpointGamePiece {
        std::string type;
        std::string color;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGamePiece, type, color);

    struct EndpointGameState {
        std::optional<EndpointGamePiece> piece;
        EndpointGamePosition position;
    };

    void from_json(const nlohmann::json &j, EndpointGameState &p);

    void to_json(nlohmann::json &j, const EndpointGameState &p);

    struct EndpointGameMove {
        std::string color;
        int source;
        int target;
        std::string move_type;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGameMove, color, source, target, move_type);

    struct EndpointGameBoard {
        std::vector<EndpointGameState> state;
        std::vector<EndpointGameMove> moves;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGameBoard, state, moves);

    struct EndpointGameReply {
        EndpointGameBoard board;
        std::string white;
        std::string black;
        bool game_over;
        std::optional<std::string> winner;
    };

    void from_json(const nlohmann::json &j, EndpointGameReply &p);

    void to_json(nlohmann::json &j, const EndpointGameReply &p);

    struct EndpointGameRequest {
        int source;
        int target;
        std::string move_type;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EndpointGameRequest, source, target, move_type);
}
#endif //CHESSTASTIC_NETWORKINGENDPOINTDATAMODELS_H
