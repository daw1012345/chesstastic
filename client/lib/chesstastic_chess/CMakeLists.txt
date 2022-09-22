add_library(chesstastic_chess_lib
        src/ChessBoard.cpp src/ChessPiece.cpp include/chesstastic_chess/ChessMoveValidationService.h include/chesstastic_chess/ChessMove.h src/ChessMoveValidationService.cpp src/ChessMove.cpp include/chesstastic_chess/Position.h src/Position.cpp)

target_include_directories(chesstastic_chess_lib
        PRIVATE include/chesstastic_chess
        INTERFACE include/
        )