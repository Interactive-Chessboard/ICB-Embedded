// test_helper.hpp
#include "unity.h"
#include "hardware.hpp"
#include "chess.hpp"


inline void TEST_ASSERT_EQUAL_LED_COLOR(const LedColor &expected, const LedColor &actual)
{
    TEST_ASSERT_EQUAL(expected.red, actual.red);
    TEST_ASSERT_EQUAL(expected.green, actual.green);
    TEST_ASSERT_EQUAL(expected.blue, actual.blue);
}


inline void TEST_ASSERT_EQUAL_CHESS_GAME(const ChessGame &expected, const ChessGame &actual)
{
    TEST_ASSERT_EQUAL(expected.player_turn, actual.player_turn);
    TEST_ASSERT_EQUAL(expected.board.size(), actual.board.size());
    for (int i = 0; i < expected.board.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.board.at(i).color, actual.board.at(i).color);
        TEST_ASSERT_EQUAL(expected.board.at(i).piece_type, actual.board.at(i).piece_type);
    }
    TEST_ASSERT_EQUAL(expected.castle.size(), actual.castle.size());
    for (int i = 0; i < expected.castle.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.castle.at(i), actual.castle.at(i));
    }
    TEST_ASSERT_EQUAL(expected.en_passant, actual.en_passant);
}


inline void TEST_ASSERT_UINT64_T(const uint64_t &expected, const uint64_t &actual)
{
    TEST_ASSERT_EQUAL_MEMORY(expected, actual, sizeof(uint64_t));
}
