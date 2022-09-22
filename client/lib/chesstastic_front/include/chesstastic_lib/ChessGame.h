//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSGAME_H
#define CHESSTASTIC_CHESSGAME_H

#include <QObject>
#include <chesstastic_chess/ChessBoard.h>
#include "CameraFeedback.h"
#include "ChesstasticAPIService.h"
#include "ChessMovePlausibilityFilter.h"

enum class ChessGameState {
    INIT, OUR_MOVE, THEIR_MOVE, OVER
};

class ChessGame : public QObject {
Q_OBJECT
signals:
// When board changes, send it over together with which side we are on (don't render our pieces)
    void board_changed(ChessBoard, ChessPieceSide);

// Send which side goes next
    void next_turn(ChessPieceSide);

// Game end (passes the optional winner)
    void game_end(std::optional<ChessPieceSide>);

// Passes a side to username mapping
    void player_pair_changed(std::map<ChessPieceSide, std::string>);

public:
    ChessGame(int api_game, std::shared_ptr<CameraFeedback> f,
              std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> api) : id(api_game), feedback(f),
                                                                                   api(api) {}

    void start();

private:
    std::optional<ChesstasticNetworking::EndpointGameReply> get_remote_state();

    void init();

    void run();

    void our_move();

    void their_move();

    void update_board(ChesstasticNetworking::EndpointGameReply);

    ChessBoard remote_to_local(ChesstasticNetworking::EndpointGameReply);

    ChessPieceSide to_local_side_format(std::string side);

    ChessGameState state = ChessGameState::INIT;

    std::thread th;
    int id;

    ChessPieceSide us;
    std::string username;
    std::string oponent;

    ChessBoard current_board;
    std::shared_ptr<CameraFeedback> feedback;
    std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> api;

    ChessMovePlausibilityFilter move_check_service;
};


#endif //CHESSTASTIC_CHESSGAME_H
