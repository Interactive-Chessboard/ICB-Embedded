// test_helper.hpp
#include "unity.h"
#include "hardware.hpp"
#include "chess.hpp"


void TEST_ASSERT_EQUAL_LED_COLOR(const LedColor &expected, const LedColor &actual)
{
    TEST_ASSERT_EQUAL(expected.red, actual.red);
    TEST_ASSERT_EQUAL(expected.green, actual.green);
    TEST_ASSERT_EQUAL(expected.blue, actual.blue);
}


void TEST_ASSERT_EQUAL_CHESS_GAME(const ChessGame &expected, const ChessGame &actual)
{
    TEST_ASSERT_EQUAL_MEMORY(&expected, &actual, sizeof(ChessGame));
}
