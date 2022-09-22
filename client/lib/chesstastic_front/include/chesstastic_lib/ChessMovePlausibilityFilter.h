//
// Created by Dawid Kulikowski on 09/11/2021.
//

#ifndef CHESSTASTIC_CHESSMOVEPLAUSIBILITYFILTER_H
#define CHESSTASTIC_CHESSMOVEPLAUSIBILITYFILTER_H

#include <optional>
#include <vector>
#include "chesstastic_chess/ChessBoard.h"
#include "chesstastic_chess/ChessMoveValidationService.h"

class ChessMovePlausibilityFilter {
public:
    std::optional<std::tuple<std::tuple<int, int>, std::string>> get_possible_move(std::vector<int>, ChessBoard, ChessPieceSide);
private:
    std::vector<std::tuple<int, int>> get_simple_possible_moves(std::vector<int>, ChessBoard, ChessPieceSide);
    bool is_short_castle(std::vector<int>, ChessPieceSide);
    bool is_long_castle(std::vector<int>, ChessPieceSide);
    bool is_promotion(std::tuple<int, int>, ChessBoard);

    ChessMoveValidationService validation;
};


#endif //CHESSTASTIC_CHESSMOVEPLAUSIBILITYFILTER_H
