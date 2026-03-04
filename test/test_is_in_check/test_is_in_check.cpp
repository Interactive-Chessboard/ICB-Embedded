// test_is_in_check
#include <unity.h>
#include "../test_helper.hpp"
#include "chess.hpp"


void test_is_in_check_no_king_white()
{
    ChessGame game = chessgameFromFen("rnbq1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1BNR w KQkq - 0 1");

    bool result = Chess::isInCheck(game);
    TEST_ASSERT_EQUAL(false, result);
}


void test_is_in_check_no_king_black()
{
    ChessGame game = chessgameFromFen("rnbq1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1BNR b KQkq - 0 1");

    bool result = Chess::isInCheck(game);
    TEST_ASSERT_EQUAL(false, result);
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_is_in_check_no_king_white);
    RUN_TEST(test_is_in_check_no_king_black);

    // I need to find 1000s of fen strings and make the tests automatically with python
    // Actually, I need a c++ fen to board fucntion in test helper

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