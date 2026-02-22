// test_make_move
#include <unity.h>
#include "../test_helper.hpp"
#include "make_move/make_move.hpp"


void test_make_move_offline()
{
    ChessGame game; // rnbqk2r/ppp2ppp/3p1n2/4p3/1bP1P3/2N3P1/PP1P1PB1/R2QK1NR b KQkq - 0 1
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 5;
    int past_move_to = 14;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xbbd722680814e7f9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680834e7b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(57, move.from_square);
    TEST_ASSERT_EQUAL(42, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(5) = LedColor(0, 0, 255);
    leds1.at(14) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(5) = LedColor(0, 0, 255);
    leds2.at(14) = LedColor(0, 0, 255);
    leds2.at(57) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 57)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_offline_stop()
{
    ChessGame game;
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 5;
    int past_move_to = 14;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xbbd722680814e7f9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680814e7b9ULL,
        0xbbd722680834e7b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &MakeMove::startOffline, &make_move, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    Move move = future.get();

    TEST_ASSERT_EQUAL(-1, move.from_square);
    TEST_ASSERT_EQUAL(-1, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(5) = LedColor(0, 0, 255);
    leds1.at(14) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(5) = LedColor(0, 0, 255);
    leds2.at(14) = LedColor(0, 0, 255);
    leds2.at(57) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 57)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_offline_invalid_start_position()
{
    ChessGame game;
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 5;
    int past_move_to = 14;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(-1, move.from_square);
    TEST_ASSERT_EQUAL(-1, move.to_square);
    TEST_ASSERT_EQUAL(0, hardware_mock.set_led_queue.size());
}


void test_make_move_online()
{
    ExtractMakeMove extract_make_move; // r1bqk2r/ppp2pp1/3p1n2/4p3/1bP1P2p/2N3PP/PP1P1PB1/R2QK2R w KQkq - 0 1
    extract_make_move.game.player_turn = Color::White;
    extract_make_move.game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    extract_make_move.game.castle = {'K', 'Q', 'k', 'q'};
    extract_make_move.game.en_passant = -1;
    extract_make_move.game.winner = Winner::Nil;
    extract_make_move.past_move_color = LedColor(1, 1, 1);
    extract_make_move.lifted_square_color = LedColor(50, 50, 50);
    extract_make_move.legal_moves_color = LedColor(100, 100, 100);
    extract_make_move.illegal_moves_color = LedColor(200, 200, 200);

    extract_make_move.past_move_from = 57;
    extract_make_move.past_move_to = 42;
    extract_make_move.timeout = 1000000;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xb9de23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da236d0834e6b9ULL
    };

    MakeMove make_move(extract_make_move);

    std::atomic<bool> active{true};
    std::string move_str = make_move.startOnline(active);

    TEST_ASSERT_EQUAL_STRING("ok, \"move_from\": 13, \"move_to\": 29\"", move_str.c_str());

    std::array<LedColor, 64> leds1;
    leds1.at(57) = LedColor(1, 1, 1);
    leds1.at(42) = LedColor(1, 1, 1);

    std::array<LedColor, 64> leds2;
    leds2.at(57) = LedColor(1, 1, 1);
    leds2.at(42) = LedColor(1, 1, 1);
    leds2.at(13) = LedColor(50, 50, 50);
    leds2.at(21) = LedColor(100, 100, 100);
    leds2.at(29) = LedColor(100, 100, 100);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_online_stop()
{
    ExtractMakeMove extract_make_move;
    extract_make_move.game.player_turn = Color::White;
    extract_make_move.game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    extract_make_move.game.castle = {'K', 'Q', 'k', 'q'};
    extract_make_move.game.en_passant = -1;
    extract_make_move.game.winner = Winner::Nil;
    extract_make_move.past_move_color = LedColor(1, 1, 1);
    extract_make_move.lifted_square_color = LedColor(50, 50, 50);
    extract_make_move.legal_moves_color = LedColor(100, 100, 100);
    extract_make_move.illegal_moves_color = LedColor(200, 200, 200);

    extract_make_move.past_move_from = 57;
    extract_make_move.past_move_to = 42;
    extract_make_move.timeout = 1000000;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xb9de23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da236d0834e6b9ULL
    };

    MakeMove make_move(extract_make_move);

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &MakeMove::startOnline, &make_move, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    std::string move_str = future.get();

    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", move_str.c_str());

    std::array<LedColor, 64> leds1;
    leds1.at(57) = LedColor(1, 1, 1);
    leds1.at(42) = LedColor(1, 1, 1);

    std::array<LedColor, 64> leds2;
    leds2.at(57) = LedColor(1, 1, 1);
    leds2.at(42) = LedColor(1, 1, 1);
    leds2.at(13) = LedColor(50, 50, 50);
    leds2.at(21) = LedColor(100, 100, 100);
    leds2.at(29) = LedColor(100, 100, 100);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_online_timeout()
{
    ExtractMakeMove extract_make_move;
    extract_make_move.game.player_turn = Color::White;
    extract_make_move.game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    extract_make_move.game.castle = {'K', 'Q', 'k', 'q'};
    extract_make_move.game.en_passant = -1;
    extract_make_move.game.winner = Winner::Nil;
    extract_make_move.past_move_color = LedColor(1, 1, 1);
    extract_make_move.lifted_square_color = LedColor(50, 50, 50);
    extract_make_move.legal_moves_color = LedColor(100, 100, 100);
    extract_make_move.illegal_moves_color = LedColor(200, 200, 200);

    extract_make_move.past_move_from = 57;
    extract_make_move.past_move_to = 42;
    extract_make_move.timeout = 5;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xb9de23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da23690834e6b9ULL,
        0xb9da236d0834e6b9ULL
    };

    MakeMove make_move(extract_make_move);

    std::atomic<bool> active{true};
    std::string move_str = make_move.startOnline(active);

    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", move_str.c_str());

    std::array<LedColor, 64> leds1;
    leds1.at(57) = LedColor(1, 1, 1);
    leds1.at(42) = LedColor(1, 1, 1);

    std::array<LedColor, 64> leds2;
    leds2.at(57) = LedColor(1, 1, 1);
    leds2.at(42) = LedColor(1, 1, 1);
    leds2.at(13) = LedColor(50, 50, 50);
    leds2.at(21) = LedColor(100, 100, 100);
    leds2.at(29) = LedColor(100, 100, 100);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_online_invalid_start_position()
{
    ExtractMakeMove extract_make_move;
    extract_make_move.game.player_turn = Color::White;
    extract_make_move.game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    extract_make_move.game.castle = {'K', 'Q', 'k', 'q'};
    extract_make_move.game.en_passant = -1;
    extract_make_move.game.winner = Winner::Nil;
    extract_make_move.past_move_color = LedColor(1, 1, 1);
    extract_make_move.lifted_square_color = LedColor(50, 50, 50);
    extract_make_move.legal_moves_color = LedColor(100, 100, 100);
    extract_make_move.illegal_moves_color = LedColor(200, 200, 200);

    extract_make_move.past_move_from = 57;
    extract_make_move.past_move_to = 42;
    extract_make_move.timeout = 1000000;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL
    };

    MakeMove make_move(extract_make_move);

    std::atomic<bool> active{true};
    std::string move_str = make_move.startOnline(active);

    TEST_ASSERT_EQUAL_STRING("error, original position not set", move_str.c_str());
    TEST_ASSERT_EQUAL(0, hardware_mock.set_led_queue.size());
}


void test_make_move_override_past_move_from()
{
    ChessGame game; // 8/1p4p1/p3k1rb/6n1/P6p/1P2P2P/4K2P/3R4 w - - 0 1
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
    leds2.at(3) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 3)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_override_past_move_to()
{
    ChessGame game; // r1bqk2r/1pp3pn/p2p1p2/8/1PPPpPpp/P1N1P2P/1B1Q2B1/R3K2R w KQkq - 0 1
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 36;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8942b17f009463b9ULL,
        0xa942b17f009463b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(11, move.from_square);
    TEST_ASSERT_EQUAL(2, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(36) = LedColor(0, 0, 255);
    leds2.at(11) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 11)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_no_past_move()
{
    ChessGame game; // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = -1;
    int past_move_to = -1;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL,
        0xfff700000000ffffULL,
        0xfff700080000ffffULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(12, move.from_square);
    TEST_ASSERT_EQUAL(28, move.to_square);

    std::array<LedColor, 64> leds1;

    std::array<LedColor, 64> leds2;
    leds2.at(12) = LedColor(255, 255, 0);
    leds2.at(20) = LedColor(0, 255, 0);
    leds2.at(28) = LedColor(0, 255, 0);

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_capture()
{
    ChessGame game; // r1bqk2r/1pp3pn/p2p1p2/8/1PPPpPpp/P1N1P2P/1B1Q2B1/R3K2R w KQkq - 0 1
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 36;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8942b17f009463b9ULL,
        0x8942b17b009463b9ULL,
        0x8942b17f009463b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(11, move.from_square);
    TEST_ASSERT_EQUAL(29, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(36) = LedColor(0, 0, 255);
    leds2.at(11) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 11)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(36) = LedColor(0, 0, 255);
    leds3.at(11) = LedColor(255, 255, 0);
    leds3.at(29) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_place_back_lifted()
{
    ChessGame game; // r1bqk2r/1pp3pn/p2p1p2/8/1PPPpPpp/P1N1P2P/1B1Q2B1/R3K2R w KQkq - 0 1
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 36;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8942b17f009463b9ULL,
        0x8952b17f009463b9ULL,
        0x8942b17f009463b9ULL,
        0x8942b17b009463b9ULL,
        0x8942b17f009463b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(11, move.from_square);
    TEST_ASSERT_EQUAL(29, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(36) = LedColor(0, 0, 255);
    leds2.at(11) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 11)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(36) = LedColor(0, 0, 255);
    leds3.at(11) = LedColor(255, 255, 0);
    leds3.at(29) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_lift_piece_no_legal_moves()
{
    ChessGame game; // rnbqkbnr/pppppppp/8/8/2P5/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 10;
    int past_move_to = 26;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffdf00200000ffffULL,
        0xffdf00200000fffeULL,
        0xffdf00200000ffffULL,
        0xffdf00200000fffdULL,
        0xffdf00200004fffdULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(62, move.from_square);
    TEST_ASSERT_EQUAL(45, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(10) = LedColor(0, 0, 255);
    leds1.at(26) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(10) = LedColor(0, 0, 255);
    leds2.at(26) = LedColor(0, 0, 255);
    leds2.at(63) = LedColor(255, 255, 0);

    std::array<LedColor, 64> leds3;
    leds3.at(10) = LedColor(0, 0, 255);
    leds3.at(26) = LedColor(0, 0, 255);
    leds3.at(62) = LedColor(255, 255, 0);
    leds3.at(45) = LedColor(0, 255, 0);
    leds3.at(47) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_lift_capture_piece_before_lifted_piece()
{
    ChessGame game; // r1bqk2r/1pp3pn/p2p1p2/8/1PPPpPpp/P1N1P2P/1B1Q2B1/R3K2R w KQkq - 0 1
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'K', 'Q', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 36;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8952b17b009463b9ULL,
        0x8942b17b009463b9ULL,
        0x8942b17f009463b9ULL
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(11, move.from_square);
    TEST_ASSERT_EQUAL(29, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(36) = LedColor(0, 0, 255);
    leds2.at(11) = LedColor(255, 255, 0);
    leds2.at(29) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_lift_capture_piece_before_other_piece()
{
    ChessGame game; // r1nq1rk1/3b1pbn/3p2p1/3Pp3/ppN1P1Pp/1P2BP1P/PK3Q2/2NR1BR1 b - - 1 21
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'.', '.', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 11;
    int past_move_to = 26;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x36c44deb181217b6ULL, // start
        0x36c40deb181217b6ULL, // lift opponent pawn b3
        0x36c40deb181217b4ULL, // lift king g8
        0x36c44deb181217b4ULL, // place back b3 pawn
        0x36c44deb181217b6ULL, // place back g8 king
        0x36c44d6b181217b6ULL, // lift pawn on a4
        0x36c40d6b181217b6ULL, // lift opponent pawn b3
        0x36c44d6b181217b6ULL // end
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(24, move.from_square);
    TEST_ASSERT_EQUAL(17, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(11) = LedColor(0, 0, 255);
    leds1.at(26) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(11) = LedColor(0, 0, 255);
    leds2.at(26) = LedColor(0, 0, 255);
    leds2.at(62) = LedColor(255, 255, 0);
    leds2.at(63) = LedColor(0, 255, 0);
    leds2.at(17) = LedColor(255, 0, 0);

    std::array<LedColor, 64> leds3;
    leds3.at(11) = LedColor(0, 0, 255);
    leds3.at(26) = LedColor(0, 0, 255);
    leds3.at(62) = LedColor(255, 255, 0);
    leds3.at(63) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;
    leds4.at(11) = LedColor(0, 0, 255);
    leds4.at(26) = LedColor(0, 0, 255);
    leds4.at(24) = LedColor(255, 255, 0);
    leds4.at(16) = LedColor(0, 255, 0);
    leds4.at(17) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds5;
    leds5.at(11) = LedColor(0, 0, 255);
    leds5.at(26) = LedColor(0, 0, 255);
    leds5.at(24) = LedColor(255, 255, 0);
    leds5.at(17) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds6;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds1, leds4, leds5, leds6};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_simultaneous_lift_opponent_smaller_index()
{
    ChessGame game; // 5rk1/5p2/3p1qp1/3Pp1n1/npB1P1P1/rN1R1P1P/P4Q2/1KR5 b - - 4 31
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'.', '.', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 3;
    int past_move_to = 19;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x6084d5ea1a160406ULL, // start
        0x6084d5e218160406ULL, // lift black knight g5 and white pawn e4
        0x6084d5ea18160406ULL // knight g5 captures on e4
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(38, move.from_square);
    TEST_ASSERT_EQUAL(28, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(3) = LedColor(0, 0, 255);
    leds1.at(19) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(3) = LedColor(0, 0, 255);
    leds2.at(19) = LedColor(0, 0, 255);
    leds2.at(38) = LedColor(255, 255, 0);
    leds2.at(28) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_simultaneous_lift_opponent_larger_index()
{
    ChessGame game; // 5rk1/5p2/3p1qp1/3Pp3/npB1n1P1/rN1R1P1P/P4Q2/1KR5 w - - 0 32
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'.', '.', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 38;
    int past_move_to = 28;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x6084d5ea18160406ULL, // start
        0x6084d1e218160406ULL, // lift white pawn f3 and black knight e4
        0x6084d1ea18160406ULL // pawn captures on e4
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(21, move.from_square);
    TEST_ASSERT_EQUAL(28, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(38) = LedColor(0, 0, 255);
    leds1.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(38) = LedColor(0, 0, 255);
    leds2.at(21) = LedColor(255, 255, 0);
    leds2.at(28) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_white_king_castle()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x81da2d0a599217b6ULL, // lift king
        0x80da2d0a599217b6ULL, // lift rook
        0x82da2d0a599217b6ULL, // place king
        0x86da2d0a599217b6ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(6, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(49) = LedColor(0, 0, 255);
    leds3.at(33) = LedColor(0, 0, 255);
    leds3.at(4) = LedColor(0, 255, 0);
    leds3.at(6) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_white_queen_castle()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x81da2d0a599217b6ULL, // lift king
        0x01da2d0a599217b6ULL, // lift rook
        0x21da2d0a599217b6ULL, // place king
        0x31da2d0a599217b6ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(2, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(49) = LedColor(0, 0, 255);
    leds3.at(33) = LedColor(0, 0, 255);
    leds3.at(4) = LedColor(0, 255, 0);
    leds3.at(2) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_black_king_castle()
{
    ChessGame game; // r3k2r/pppbqppp/4pn2/2b5/5N2/5B2/PPP2PPP/RNBQ1RK1 b kq - 3 11
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'.', '.', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 12;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xf6e70404200cff89ULL, // start
        0xf6e70404200cff81ULL, // lift king
        0xf6e70404200cff80ULL, // lift rook
        0xf6e70404200cff82ULL, // place king
        0xf6e70404200cff86ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(60, move.from_square);
    TEST_ASSERT_EQUAL(62, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(12) = LedColor(0, 0, 255);
    leds2.at(29) = LedColor(0, 0, 255);
    leds2.at(60) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 60)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(12) = LedColor(0, 0, 255);
    leds3.at(29) = LedColor(0, 0, 255);
    leds3.at(60) = LedColor(0, 255, 0);
    leds3.at(62) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_black_queen_castle()
{
    ChessGame game; // r3k2r/pppbqppp/4pn2/2b5/5N2/5B2/PPP2PPP/RNBQ1RK1 b kq - 3 11
    game.player_turn = Color::Black;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}
    };
    game.castle = {'.', '.', 'k', 'q'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 12;
    int past_move_to = 29;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xf6e70404200cff89ULL, // start
        0xf6e70404200cff81ULL, // lift king
        0xf6e70404200cff01ULL, // lift rook
        0xf6e70404200cff21ULL, // place king
        0xf6e70404200cff31ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(60, move.from_square);
    TEST_ASSERT_EQUAL(58, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(12) = LedColor(0, 0, 255);
    leds2.at(29) = LedColor(0, 0, 255);
    leds2.at(60) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 60)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(12) = LedColor(0, 0, 255);
    leds3.at(29) = LedColor(0, 0, 255);
    leds3.at(60) = LedColor(0, 255, 0);
    leds3.at(58) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_castle_lift_king_complete_rook()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x81da2d0a599217b6ULL, // lift king
        0x80da2d0a599217b6ULL, // lift rook
        0x84da2d0a599217b6ULL, // place rook
        0x86da2d0a599217b6ULL, // place king
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(6, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_castle_complete_king_before_lift_rook()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x81da2d0a599217b6ULL, // lift king
        0x83da2d0a599217b6ULL, // place king
        0x82da2d0a599217b6ULL, // lift rook
        0x86da2d0a599217b6ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(6, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(49) = LedColor(0, 0, 255);
    leds3.at(33) = LedColor(0, 0, 255);
    leds3.at(4) = LedColor(0, 255, 0);
    leds3.at(6) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_castle_simultaneous_lift_king_smaller_index()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x80da2d0a599217b6ULL, // lift king and rook
        0x82da2d0a599217b6ULL, // place king
        0x86da2d0a599217b6ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(6, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(49) = LedColor(0, 0, 255);
    leds3.at(33) = LedColor(0, 0, 255);
    leds3.at(4) = LedColor(0, 255, 0);
    leds3.at(6) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_castle_simultaneous_lift_king_larger_index()
{
    ChessGame game; // r1bq1rk1/3n1pbn/p2p2p1/1p1Pp2p/4P1P1/2N1BP1P/PP1QN1B1/R3K2R w KQ - 0 14
    game.player_turn = Color::White;
    game.board = {
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::King}, Piece{Color::Nil, PieceType::Nil}
    };
    game.castle = {'K', 'Q', '.', '.'};
    game.en_passant = -1;
    game.winner = Winner::Nil;

    std::vector<Move> legal_moves = Chess::generateLegalMoves(game);
    int past_move_from = 49;
    int past_move_to = 33;

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x89da2d0a599217b6ULL, // start
        0x01da2d0a599217b6ULL, // lift king and rook
        0x21da2d0a599217b6ULL, // place king
        0x31da2d0a599217b6ULL, // place rook
    };

    MakeMove make_move(game, legal_moves, past_move_from, past_move_to);

    std::atomic<bool> active{true};
    Move move = make_move.startOffline(active);

    TEST_ASSERT_EQUAL(4, move.from_square);
    TEST_ASSERT_EQUAL(2, move.to_square);

    std::array<LedColor, 64> leds1;
    leds1.at(49) = LedColor(0, 0, 255);
    leds1.at(33) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(49) = LedColor(0, 0, 255);
    leds2.at(33) = LedColor(0, 0, 255);
    leds2.at(4) = LedColor(255, 255, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 4)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(49) = LedColor(0, 0, 255);
    leds3.at(33) = LedColor(0, 0, 255);
    leds3.at(4) = LedColor(0, 255, 0);
    leds3.at(2) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
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

    RUN_TEST(test_make_move_offline);
    RUN_TEST(test_make_move_offline_stop);
    RUN_TEST(test_make_move_offline_invalid_start_position);
    //RUN_TEST(test_make_move_offline_promotion);
    //RUN_TEST(test_make_move_offline_promotion_not_queen);
    RUN_TEST(test_make_move_online);
    RUN_TEST(test_make_move_online_stop);
    RUN_TEST(test_make_move_online_timeout);
    RUN_TEST(test_make_move_online_invalid_start_position);
    RUN_TEST(test_make_move_override_past_move_from);
    RUN_TEST(test_make_move_override_past_move_to);
    RUN_TEST(test_make_move_no_past_move);
    RUN_TEST(test_make_move_capture);
    RUN_TEST(test_make_move_place_back_lifted);
    RUN_TEST(test_make_move_lift_piece_no_legal_moves);
    RUN_TEST(test_make_move_lift_capture_piece_before_lifted_piece);
    RUN_TEST(test_make_move_lift_capture_piece_before_other_piece);
    RUN_TEST(test_make_move_simultaneous_lift_opponent_smaller_index);
    RUN_TEST(test_make_move_simultaneous_lift_opponent_larger_index);
    RUN_TEST(test_make_move_white_king_castle);
    RUN_TEST(test_make_move_white_queen_castle);
    RUN_TEST(test_make_move_black_king_castle);
    RUN_TEST(test_make_move_black_queen_castle);
    RUN_TEST(test_make_move_castle_lift_king_complete_rook);
    RUN_TEST(test_make_move_castle_complete_king_before_lift_rook);
    RUN_TEST(test_make_move_castle_simultaneous_lift_king_smaller_index);
    RUN_TEST(test_make_move_castle_simultaneous_lift_king_larger_index);
    //RUN_TEST(test_make_move_white_right_en_passant);
    //RUN_TEST(test_make_move_white_left_en_passant);
    //RUN_TEST(test_make_move_white_en_passant_simultaneous_lift);
    //RUN_TEST(test_make_move_black_right_en_passant);
    //RUN_TEST(test_make_move_black_left_en_passant);
    //RUN_TEST(test_make_move_black_en_passant_simultaneous_lift);
    //RUN_TEST(test_make_move_en_passant_complete_before_opponent_pawn_lift);
    //RUN_TEST(test_make_move_en_passant_lift_captured_pawn_first);
    //RUN_TEST(test_make_move_lift_two_pieces);
    //RUN_TEST(test_make_move_lift_two_opponent_pieces);
    //RUN_TEST(test_make_move_illegal_lifted);
    //RUN_TEST(test_make_move_multiple_illegal_lifted);
    //RUN_TEST(test_make_move_illegal_placed);
    //RUN_TEST(test_make_move_multiple_illegal_placed);
    //RUN_TEST(test_make_move_illegal_move_overrides_past_move);

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