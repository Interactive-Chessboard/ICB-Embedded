// test_get_clock_winner.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "chess.hpp"


void test_white_checkmate()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/1ppp1Qpp/8/4p3/p1B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");

    Winner winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));
    TEST_ASSERT_EQUAL(Winner::White, winner);
}


void test_black_checkmate()
{
    ChessGame game = chessgameFromFen("2bq1bnr/1pp4p/r1k3p1/pn1pp2K/2B1P3/8/PPPP1PPP/RNB3NR w - - 0 12");

    Winner winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));
    TEST_ASSERT_EQUAL(Winner::Black, winner);
}


void test_stalemate()
{
    ChessGame game = chessgameFromFen("8/8/8/8/8/5K2/6R1/7k b - - 1 1");

    Winner winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));
    TEST_ASSERT_EQUAL(Winner::Draw, winner);
}


void test_insufficient_materials()
{
    ChessGame game = chessgameFromFen("8/8/8/8/8/5K2/8/7k w - - 1 1");

    Winner winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));
    TEST_ASSERT_EQUAL(Winner::Draw, winner);
}


void test_no_winner()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Winner winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));
    TEST_ASSERT_EQUAL(Winner::Nil, winner);
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_white_checkmate);
    RUN_TEST(test_black_checkmate);
    RUN_TEST(test_stalemate);
    RUN_TEST(test_insufficient_materials);

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
