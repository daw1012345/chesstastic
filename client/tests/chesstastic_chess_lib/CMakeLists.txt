# Now simply link against gtest or gtest_main as needed. Eg

add_executable(chesstastic_chess_lib_test
        src/ChessMoveValidationServiceTest.cpp src/ChessBoardTest.cpp src/PositionTest.cpp src/ChessPieceTest.cpp src/ChessMoveTest.cpp)

target_link_libraries(chesstastic_chess_lib_test chesstastic_chess_lib gtest_main gmock)