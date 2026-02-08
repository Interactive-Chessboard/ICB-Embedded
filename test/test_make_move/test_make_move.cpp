// test_make_move
#include <unity.h>
#include "../test_helper.hpp"
#include "make_move/make_move.hpp"


void test_make_move_override_past_move_from()
{
    ChessGame game;
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'.', '.', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 43;
    int past_move_to = 42;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x2002094b82ee4200ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(3, move.from_square);
    TEST_ASSERT_EQUAL(2, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(42) = LedColor(0, 0, 255);
    leds1.at(43) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(42) = LedColor(0, 0, 255);
    leds2.at(3) = LedColor(0, 230, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 3)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


// runs before each test
void setUp()
{
    setUpMockHardware();
}


void runTests()
{
    UNITY_BEGIN();

    //RUN_TEST(test_make_move_offline);
    //RUN_TEST(test_make_move_offline_stop);
    //RUN_TEST(test_make_move_offline_promotion);
    //RUN_TEST(test_make_move_online);
    //RUN_TEST(test_make_move_online_stop);
    //RUN_TEST(test_make_move_online_timeout);
    //RUN_TEST(test_make_move_no_past_move);
    //RUN_TEST(test_make_move_capture);
    //RUN_TEST(test_make_move_place_back_lifted);
    RUN_TEST(test_make_move_override_past_move_from);
    //RUN_TEST(test_make_move_override_past_move_to);
    //RUN_TEST(test_make_move_simultaneous_lift);
    //RUN_TEST(test_make_move_lift_piece_no_legal_moves);
    //RUN_TEST(test_make_move_white_king_castle);
    //RUN_TEST(test_make_move_white_queen_castle);
    //RUN_TEST(test_make_move_black_king_castle);
    //RUN_TEST(test_make_move_black_queen_castle);
    //RUN_TEST(test_make_move_castle_complete_king_before_rook);
    //RUN_TEST(test_make_move_castle_simultaneous_lift);
    //RUN_TEST(test_make_move_white_right_en_passant);
    //RUN_TEST(test_make_move_white_left_en_passant);
    //RUN_TEST(test_make_move_black_right_en_passant);
    //RUN_TEST(test_make_move_black_left_en_passant);
    //RUN_TEST(test_make_move_en_passant_complete_before_opponent_pawn_lift);
    //RUN_TEST(test_make_move_en_passant_simultaneous_lift);
    //RUN_TEST(test_make_move_lift_second_piece);
    //RUN_TEST(test_make_move_illegal_lifted);
    //RUN_TEST(test_make_move_multiple_illegal_lifted);
    //RUN_TEST(test_make_move_illegal_placed);
    //RUN_TEST(test_make_move_multiple_illegal_placed);

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