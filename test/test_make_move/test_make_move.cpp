// test_make_move
#include <unity.h>
#include "../test_helper.hpp"
#include "make_move/make_move.hpp"


void test_make_move_offline()
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
    leds2.at(57) = LedColor(0, 230, 0);
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
    leds2.at(57) = LedColor(0, 230, 0);
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


void test_make_move_override_past_move_to()
{
    ChessGame game;
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
    leds2.at(11) = LedColor(0, 230, 0);
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
    ChessGame game;

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
    leds2.at(12) = LedColor(0, 230, 0);
    leds2.at(20) = LedColor(0, 255, 0);
    leds2.at(28) = LedColor(0, 255, 0);

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_capture()
{
    ChessGame game;
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
    leds2.at(11) = LedColor(0, 230, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 11)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(36) = LedColor(0, 0, 255);
    leds3.at(11) = LedColor(0, 230, 0);
    leds3.at(29) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_place_back_lifted()
{
    ChessGame game;
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
    leds2.at(11) = LedColor(0, 230, 0);
    for (Move move : legal_moves)
    {
        if (move.from_square == 11)
            leds2.at(move.to_square) = LedColor(0, 255, 0);
    }

    std::array<LedColor, 64> leds3;
    leds3.at(36) = LedColor(0, 0, 255);
    leds3.at(11) = LedColor(0, 230, 0);
    leds3.at(29) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1, leds2, leds3, leds4};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_make_move_lift_piece_no_legal_moves()
{
    ChessGame game;
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
    leds2.at(63) = LedColor(0, 230, 0);

    std::array<LedColor, 64> leds3;
    leds3.at(10) = LedColor(0, 0, 255);
    leds3.at(26) = LedColor(0, 0, 255);
    leds3.at(62) = LedColor(0, 230, 0);
    leds3.at(45) = LedColor(0, 255, 0);
    leds3.at(47) = LedColor(0, 255, 0);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds1, leds3, leds4};
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
    //RUN_TEST(test_make_move_lift_piece_no_legal_moves); // TO DO: Change logic
    //RUN_TEST(test_make_move_simultaneous_lift_opponent_smaller_index); // TO DO, feature not implemented (I think). Players should be able to lift the captured piece before their own
    //RUN_TEST(test_make_move_simultaneous_lift_opponent_bigger_index); // TO DO, feature not implemented (I think). Players should be able to lift the captured piece before their own
    //RUN_TEST(test_make_move_lift_capture_piece_before_lifted_piece); // TO DO, feature not implemented (I think). Players should be able to lift the captured piece before their own
    //RUN_TEST(test_make_move_white_king_castle);
    //RUN_TEST(test_make_move_white_queen_castle);
    //RUN_TEST(test_make_move_black_king_castle);
    //RUN_TEST(test_make_move_black_queen_castle);
    //RUN_TEST(test_make_move_castle_complete_king_before_rook);
    //RUN_TEST(test_make_move_castle_simultaneous_lift);
    //RUN_TEST(test_make_move_white_right_en_passant);
    //RUN_TEST(test_make_move_white_left_en_passant);
    //RUN_TEST(test_make_move_white_en_passant_simultaneous_lift);
    //RUN_TEST(test_make_move_black_right_en_passant);
    //RUN_TEST(test_make_move_black_left_en_passant);
    //RUN_TEST(test_make_move_black_en_passant_simultaneous_lift);
    //RUN_TEST(test_make_move_en_passant_complete_before_opponent_pawn_lift);
    //RUN_TEST(test_make_move_en_passant_lift_captured_pawn_first);
    //RUN_TEST(test_make_move_lift_second_piece);
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