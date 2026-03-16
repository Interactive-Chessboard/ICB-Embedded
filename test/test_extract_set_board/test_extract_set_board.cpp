// test_extract_set_board.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "online_game/extract_set_board/extract_set_board.hpp"



void test_functional_set_board()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr",
        "past_move": {
            "color": [0, 0, 255],
            "from": -1,
            "to": -1
        },
        "clock": {
            "active": "f",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";
    ExtractSetBoard extract_set_board;
    try
    {
        extract_set_board = extractSetBoard(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_CHESS_GAME(ChessGame(), extract_set_board.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_set_board.past_move_color);
    TEST_ASSERT_EQUAL(-1, extract_set_board.past_move_from);
    TEST_ASSERT_EQUAL(-1, extract_set_board.past_move_to);
    TEST_ASSERT_EQUAL(60000, extract_set_board.timeout);
}


void test_functional_set_board2()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": "RNBQKBNRPPPPPPP........P........................pppppppprnbqkbnr",
        "past_move": {
            "color": [0, 0, 255],
            "from": 15,
            "to": 31
        },
        "clock": {
            "active": "f",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";
    ExtractSetBoard extract_set_board;
    try
    {
        extract_set_board = extractSetBoard(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    ChessGame game;
    game.board.at(23) = game.board.at(15);
    game.board.at(15) = Piece();
    TEST_ASSERT_EQUAL_CHESS_GAME(game, extract_set_board.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_set_board.past_move_color);
    TEST_ASSERT_EQUAL(15, extract_set_board.past_move_from);
    TEST_ASSERT_EQUAL(31, extract_set_board.past_move_to);
    TEST_ASSERT_EQUAL(60000, extract_set_board.timeout);
}




void test_invalid_color()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "a",
        },
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractSetBoard(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid player color", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_functional_set_board);
    RUN_TEST(test_functional_set_board2);
    RUN_TEST(test_invalid_color);

    UNITY_END();
}


#ifdef ARDUINO
void setup()
{
    runTests();
}
void loop() {}
#else
int main()
{
    runTests();
}
#endif
