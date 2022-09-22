//
// Created by Dawid Kulikowski on 09/11/2021.
//

#include "ChessMovePlausibilityFilter.h"
#include <set>
#include <algorithm>
#include <iostream>
#include <vector>

std::optional<std::tuple<std::tuple<int, int>, std::string>> ChessMovePlausibilityFilter::get_possible_move(std::vector<int> changes, ChessBoard board, ChessPieceSide us) {
    if (changes.size() >= 5) {
        return std::nullopt;
    }

    if (changes.size() == 1) {
        int p_king_idx = changes.at(0);
        auto piece = board.get_field(p_king_idx);
        if (piece.has_value() && piece.value().get_type() == ChessPieceType::KING && piece.value().get_side() == us) {
            return std::make_optional<std::tuple<std::tuple<int, int>, std::string>>({{0, 0}, "resign"});
        }
    }

    if (is_long_castle(changes, us) && validation.isValidMove({{us, ChessPieceType::KING}, 0, 0, MoveType::CASTLE_LONG}, board)) {
        return std::make_optional<std::tuple<std::tuple<int, int>, std::string>>({{0, 0}, "castle_long"});
    }

    if (is_short_castle(changes, us) && validation.isValidMove({{us, ChessPieceType::KING}, 0, 0, MoveType::CASTLE_SHORT}, board)) {
        return std::make_optional<std::tuple<std::tuple<int, int>, std::string>>({{0, 0}, "castle_short"});
    }

    if (changes.size() >= 2) {
        auto moves = get_simple_possible_moves(changes, board, us);

        moves.erase(std::remove_if(moves.begin(), moves.end(), [&] (auto elem) {
            auto [a, b] = elem;
            auto piece = board.get_field({a}).value();

            auto check = validation.isValidMove(ChessMove{piece, a, b, MoveType::REGULAR}, board);
            std::cout << "[VALIDATION] Checking if valid: " << std::to_string(a) << " -> " << std::to_string(b) << " {" << std::boolalpha << check << "}" << std::endl;
            return !check;
        }), moves.end());


        std::cout << "[VALIDATION] Moves left: " << std::to_string(moves.size()) << std::endl;

        if (moves.size() == 1) {
            if (is_promotion(moves.at(0), board)) {
                return std::make_optional<std::tuple<std::tuple<int, int>, std::string>>({moves.at(0), "promotion_queen"});
            } else {
                return std::make_optional<std::tuple<std::tuple<int, int>, std::string>>({moves.at(0), "regular"});
            }
        }
    }

    return std::nullopt;
}

std::vector<std::tuple<int, int>> ChessMovePlausibilityFilter::get_simple_possible_moves(std::vector<int> changes, ChessBoard board, ChessPieceSide us) {
    std::vector<std::tuple<int, int>> res;
    for (auto mv : changes) {
        if (board.is_field_empty(mv) || board.get_field(mv).has_value() && board.get_field(mv).value().get_side() != us) continue;

        for (auto b : changes) {
            if ((!board.is_field_empty(b) && board.get_field(b).value().get_side() != us) || board.is_field_empty(b)) {
                res.push_back({mv, b});
            }
        }
    }

    return res;
}

bool ChessMovePlausibilityFilter::is_short_castle(std::vector<int> changes,  ChessPieceSide us) {
    const auto REQUIRED_CHANGES_WHITE = {4, 5, 6, 7};
    const auto REQUIRED_CHANGES_BLACK = {63, 62, 61, 60}; // Mirrored across x axis

    if (changes.size() != 4) return false;

    bool has_white_castle = true;
    bool has_black_castle = true;

    for (auto req_w  : REQUIRED_CHANGES_WHITE) {
        has_white_castle &= std::count(changes.begin(), changes.end(), req_w);
    }

    for (auto req_b : REQUIRED_CHANGES_BLACK) {
        has_black_castle &= std::count(changes.begin(), changes.end(), req_b);
    }

    return (has_white_castle && us == ChessPieceSide::WHITE) || (has_black_castle && us == ChessPieceSide::BLACK);
}

bool ChessMovePlausibilityFilter::is_long_castle(std::vector<int> changes, ChessPieceSide us) {
    const auto REQUIRED_CHANGES_WHITE = {0, 2, 3, 4};
    const auto REQUIRED_CHANGES_BLACK = {60, 59, 58, 56}; // Mirrored across x axis

    if (changes.size() != 4) return false;

    bool has_white_castle = true;
    bool has_black_castle = true;

    for (auto req_w : REQUIRED_CHANGES_WHITE) {
        has_white_castle &= std::count(changes.begin(), changes.end(), req_w);
    }

    for (auto req_b : REQUIRED_CHANGES_BLACK) {
        has_black_castle &= std::count(changes.begin(), changes.end(), req_b);
    }

    return (has_white_castle && us == ChessPieceSide::WHITE) || (has_black_castle && us == ChessPieceSide::BLACK);
}

bool ChessMovePlausibilityFilter::is_promotion(std::tuple<int, int> move, ChessBoard board) {
    auto [a, b] = move;
    int row_a = a / 8;
    int row_b = b / 8;

    bool has_two_top_rows_move = (row_a == 6 && row_b == 7) || (row_a == 1 && row_b == 0);
    bool is_pawn = board.get_field(a).value().get_type() == ChessPieceType::PAWN;

    return has_two_top_rows_move && is_pawn;
}

