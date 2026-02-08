// test_make_move
#include <unity.h>
#include "../test_helper.hpp"
#include "make_move/make_move.hpp"

#include <iostream>
void test_make_move_offline()
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

    int past_move_from = 43;
    int past_move_to = 42;
    std::cout << "here" << std::endl;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    std::cout << "here2" << std::endl;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x2002094b82ee4200ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);
    std::cout << "here3" << std::endl;
    std::atomic<bool> active{true};
    std::cout << "here4" << std::endl;
    Move move = make_move.startOffline(active);
    std::cout << "here4" << std::endl;
    TEST_ASSERT_EQUAL(3, move.from_square);
    TEST_ASSERT_EQUAL(2, move.to_square);
}


// runs before each test
void setUp()
{
    setUpMockHardware();
}


void runTests()
{
    UNITY_BEGIN();

    std::cout << "run test" << "\n";
    RUN_TEST(test_make_move_offline);

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