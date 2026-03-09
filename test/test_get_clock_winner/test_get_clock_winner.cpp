// test_get_clock_winner.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "chess.hpp"


void test_white_lose()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int time_white = 0;
    int time_black = 100;

    Winner winner = Chess::getClockWinner(time_white, time_black, game);
    TEST_ASSERT_EQUAL(Winner::Black, winner);
}


void test_black_lose()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int time_white = 100;
    int time_black = 0;

    Winner winner = Chess::getClockWinner(time_white, time_black, game);
    TEST_ASSERT_EQUAL(Winner::White, winner);
}


void test_white_timeout_black_insufficient_materials()
{
    ChessGame game = chessgameFromFen("k7/8/8/8/8/8/8/7K w - - 0 1");
    int time_white = 0;
    int time_black = 100;

    Winner winner = Chess::getClockWinner(time_white, time_black, game);
    TEST_ASSERT_EQUAL(Winner::Draw, winner);
}


void test_black_timeout_white_insufficient_materials()
{
    ChessGame game = chessgameFromFen("k7/8/8/8/8/8/8/7K w - - 0 1");
    int time_white = 100;
    int time_black = 0;

    Winner winner = Chess::getClockWinner(time_white, time_black, game);
    TEST_ASSERT_EQUAL(Winner::Draw, winner);
}


void test_no_timeout()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int time_white = 100;
    int time_black = 100;

    Winner winner = Chess::getClockWinner(time_white, time_black, game);
    TEST_ASSERT_EQUAL(Winner::Draw, winner);
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_white_lose);
    RUN_TEST(test_black_lose);
    RUN_TEST(test_white_timeout_black_insufficient_materials);
    RUN_TEST(test_black_timeout_white_insufficient_materials);
    RUN_TEST(test_no_timeout);


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
