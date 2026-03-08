// test_insufficient_materials.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "chess.hpp"


void test_only_kings_white()
{
    ChessGame game = chessgameFromFen("k7/8/8/8/8/8/8/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_only_kings_black()
{
    ChessGame game = chessgameFromFen("k7/8/8/8/8/8/8/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_knight_white()
{
    ChessGame game = chessgameFromFen("k1n5/8/8/8/8/8/8/5N1K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_knight_black()
{
    ChessGame game = chessgameFromFen("k1n5/8/8/8/8/8/8/5N1K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_bishop_white()
{
    ChessGame game = chessgameFromFen("k7/b7/8/8/8/8/7B/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_bishop_black()
{
    ChessGame game = chessgameFromFen("k7/b7/8/8/8/8/7B/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_bishop_knight_white()
{
    ChessGame game = chessgameFromFen("k1n5/b7/8/8/8/8/7B/5N1K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_king_bishop_knight_black()
{
    ChessGame game = chessgameFromFen("k1n5/b7/8/8/8/8/7B/5N1K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(true, result);
}


void test_two_knights_white()
{
    ChessGame game = chessgameFromFen("kn6/n7/8/8/8/8/7N/6NK w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_two_knights_black()
{
    ChessGame game = chessgameFromFen("kn6/n7/8/8/8/8/7N/6NK w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_two_bishops_white()
{
    ChessGame game = chessgameFromFen("kb6/b7/8/8/8/8/7B/6BK w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_two_bishops_black()
{
    ChessGame game = chessgameFromFen("kb6/b7/8/8/8/8/7B/6BK w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_pawn_white()
{
    ChessGame game = chessgameFromFen("k7/2p5/8/8/8/8/5P2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_pawn_black()
{
    ChessGame game = chessgameFromFen("k7/2p5/8/8/8/8/5P2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_queen_white()
{
    ChessGame game = chessgameFromFen("k7/2q5/8/8/8/8/5Q2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_queen_black()
{
    ChessGame game = chessgameFromFen("k7/2q5/8/8/8/8/5Q2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_rook_white()
{
    ChessGame game = chessgameFromFen("k7/2r5/8/8/8/8/5R2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void test_rook_black()
{
    ChessGame game = chessgameFromFen("k7/2r5/8/8/8/8/5R2/7K w - - 0 1");
    Color player_color = Color::White;

    bool result = Chess::insufficientMaterials(game, player_color);
    TEST_ASSERT_EQUAL(false, result);
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_only_kings_white);
    RUN_TEST(test_only_kings_black);
    RUN_TEST(test_king_knight_white);
    RUN_TEST(test_king_knight_black);
    RUN_TEST(test_king_bishop_white);
    RUN_TEST(test_king_bishop_black);
    RUN_TEST(test_king_bishop_knight_white);
    RUN_TEST(test_king_bishop_knight_black);
    RUN_TEST(test_two_knights_white);
    RUN_TEST(test_two_knights_black);
    RUN_TEST(test_two_bishops_white);
    RUN_TEST(test_two_bishops_black);
    RUN_TEST(test_pawn_white);
    RUN_TEST(test_pawn_black);
    RUN_TEST(test_queen_white);
    RUN_TEST(test_queen_black);
    RUN_TEST(test_rook_white);
    RUN_TEST(test_rook_black);

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
