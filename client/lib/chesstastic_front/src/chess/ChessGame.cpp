//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChessGame.h"

void ChessGame::start() {
    if (this->th.joinable()) return;
    this->th = std::move(std::thread([=]() {
        this->run();
    }));

}

void ChessGame::run() {
    while (this->state != ChessGameState::OVER) {
        switch (this->state) {
            case ChessGameState::OUR_MOVE:
                std::cout << "[CHESS] STATE: OUR_MOVE" << std::endl;
                emit next_turn(this->us);
                this->our_move();
                break;
            case ChessGameState::THEIR_MOVE:
                std::cout << "[CHESS] STATE: THEIR_MOVE" << std::endl;
                emit next_turn(this->us == ChessPieceSide::WHITE ? ChessPieceSide::BLACK : ChessPieceSide::WHITE); // Invert for oponent
                this->their_move();
                break;
            case ChessGameState::INIT:
                std::cout << "[CHESS] STATE: INIT" << std::endl;
                init();
                break;
            case ChessGameState::OVER:
            default:
                std::cout << "[CHESS] STATE: OVER {LOOP}" << std::endl;
                break;
        }
    }

    std::cout << "[CHESS] STATE: OVER {RETURN}" << std::endl;

}

ChessPieceSide ChessGame::to_local_side_format(std::string side) {
    return side == "white" ? ChessPieceSide::WHITE : ChessPieceSide::BLACK;
}

void ChessGame::init() {
    auto us_future = this->api->info();
    auto us_variant = us_future.get();
    if (auto us_data = std::get_if<ChesstasticNetworking::EndpointInfoReply>(&us_variant)) {
        this->username = us_data->username;
    }

    auto remote_game_data = this->get_remote_state();
    if (remote_game_data.has_value()) {
        std::map<ChessPieceSide, std::string> usernames;
        usernames.insert({ChessPieceSide::WHITE, remote_game_data->white});
        usernames.insert({ChessPieceSide::BLACK, remote_game_data->black});

        emit player_pair_changed(usernames);

        update_board(remote_game_data.value());
    } else {
        std::cout << "[CHESS] Could not bootstrap game!" << std::endl;
    }

}

ChessBoard ChessGame::remote_to_local(ChesstasticNetworking::EndpointGameReply remote) {
    ChessBoard local;
    for (const auto &block: remote.board.state) {
        if (!block.piece.has_value()) continue;
        local.set_field(block.position.index - 1, {block.piece.value().color, block.piece.value().type});
    }

    return local;
}

void ChessGame::our_move() {
    std::tuple<int, int> accepted_move;
    std::string accepted_move_type;

    while (true) {
        auto changes = this->feedback->wait_for_move();
        auto move = this->move_check_service.get_possible_move(changes, current_board, us);
        if (move.has_value()) {
            auto [move_fields, move_type] = move.value();
            accepted_move = move_fields;
            accepted_move_type = move_type;

            break;
        }
    }

    auto[src, trgt] = accepted_move;
    std::cout << "[CHESS] We made a move decision: " << std::to_string(src) << " -> " << std::to_string(trgt)
              << std::endl;
    ChesstasticNetworking::EndpointGameRequest req;
    req.move_type = accepted_move_type;
    req.source = src + 1;
    req.target = trgt + 1;

    auto move_future = this->api.get()->move(this->id, req);
    move_future.wait();
    auto result = move_future.get();

    if (auto error = std::get_if<ChesstasticNetworking::EndpointErrorReply>(&result)) {
        std::cout << "[CHESS] Error while making move: " << error->code << " -> " << error->description;
        return;
    }

    auto remote_game_data = this->get_remote_state();
    if (remote_game_data.has_value()) {
        update_board(remote_game_data.value());
    }
}

void ChessGame::their_move() {
    auto remote_game_data = this->get_remote_state();
    if (remote_game_data.has_value()) {
        update_board(remote_game_data.value());
    }
}

std::optional<ChesstasticNetworking::EndpointGameReply> ChessGame::get_remote_state() {
    auto init_game_future = this->api->game(this->id);
    auto init_game_variant = init_game_future.get();

    if (auto init_game = std::get_if<ChesstasticNetworking::EndpointGameReply>(&init_game_variant)) {
        return *init_game;
    }

    return std::nullopt;
}

void ChessGame::update_board(ChesstasticNetworking::EndpointGameReply data) {
        this->us = this->username == data.white ? ChessPieceSide::WHITE : ChessPieceSide::BLACK;

        if (!data.board.moves.empty()) {
            auto last_move = data.board.moves.at(data.board.moves.size() - 1);
            if (to_local_side_format(last_move.color) == this->us) {
                this->state = ChessGameState::THEIR_MOVE;
            } else {
                this->state = ChessGameState::OUR_MOVE;
            }
        } else {
            this->state = this->us == ChessPieceSide::WHITE ? ChessGameState::OUR_MOVE :  ChessGameState::THEIR_MOVE;
        }

        this->current_board = remote_to_local(data);
        emit board_changed(this->current_board, this->us);

        if (!data.game_over) return;
        this->state = ChessGameState::OVER;

        if (data.winner.has_value() && data.winner.value() == this->username) {
            emit game_end(this->us);
        } else if (data.winner.has_value() && data.winner.value() != this->username) {
            emit game_end((this->us == ChessPieceSide::WHITE) ? ChessPieceSide::BLACK : ChessPieceSide::WHITE); // Take our opponent because we didn;t win
        } else {
            emit game_end(std::nullopt);
        }

}
