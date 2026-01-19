// test_extract_make_move.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "online_game/extract_make_move/extract_make_move.hpp"


void test_functional_make_move()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr",
        "castling": "KQkq",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    ExtractMakeMove extract_make_move;
    try
    {
        extract_make_move = extractMakeMove(input);
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors must be lower than 256", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_CHESS_GAME(ChessGame(), extract_make_move.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_make_move.past_move_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 230, 0), extract_make_move.lifted_square_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 255, 0), extract_make_move.legal_moves_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(255, 0, 0), extract_make_move.illegal_moves_color);
    TEST_ASSERT_EQUAL(12, extract_make_move.past_move_from);
    TEST_ASSERT_EQUAL(28, extract_make_move.past_move_to);
    TEST_ASSERT_EQUAL(6000, extract_make_move.timeout);
}


void setup()
{
    UNITY_BEGIN();

    RUN_TEST(test_functional_make_move);

    UNITY_END();
}

void loop() {}
