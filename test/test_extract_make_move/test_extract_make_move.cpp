// test_extract_make_move.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "online_game/extract_make_move/extract_make_move.hpp"


void test_functional_make_move()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr",
        "castling": "KQkq",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    ExtractMakeMove extract_make_move;
    try
    {
        extract_make_move = extractMakeMove(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_CHESS_GAME(ChessGame(), extract_make_move.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_make_move.past_move_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 230, 0), extract_make_move.lifted_square_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 255, 0), extract_make_move.legal_moves_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(255, 0, 0), extract_make_move.illegal_moves_color);
    TEST_ASSERT_EQUAL(12, extract_make_move.past_move_from);
    TEST_ASSERT_EQUAL(28, extract_make_move.past_move_to);
    TEST_ASSERT_EQUAL(60000, extract_make_move.timeout);
}


void test_functional_en_passant_make_move()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": "RNBQK.NRPPPP.PPP...................Ppb..B.......ppp..ppprn.qkbnr"
        "castling": "KQkq",
        "en_passant": 44,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    ExtractMakeMove extract_make_move;
    try
    {
        extract_make_move = extractMakeMove(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    ChessGame game;
    game.player_turn = Color::White;
    game.board = std::array<Piece, 64>{
        Piece{Color::White, PieceType::Rook}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Bishop}, Piece{Color::White, PieceType::Queen}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Black, PieceType::King}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Rook}
    },
    game.castle = std::array<char, 4>{'K', 'Q', 'k', 'q'};
    game.en_passant = 44;

    TEST_ASSERT_EQUAL_CHESS_GAME(game, extract_make_move.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_make_move.past_move_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 230, 0), extract_make_move.lifted_square_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 255, 0), extract_make_move.legal_moves_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(255, 0, 0), extract_make_move.illegal_moves_color);
    TEST_ASSERT_EQUAL(52, extract_make_move.past_move_from);
    TEST_ASSERT_EQUAL(36, extract_make_move.past_move_to);
    TEST_ASSERT_EQUAL(60000, extract_make_move.timeout);
}


void test_functional_castling_make_move()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K...",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    ExtractMakeMove extract_make_move;
    try
    {
        extract_make_move = extractMakeMove(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    ChessGame game;
    game.player_turn = Color::Black;
    game.board = std::array<Piece, 64>{
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::King}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Knight}, Piece{Color::White, PieceType::Rook},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Rook}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Bishop}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::White, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Rook}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Bishop}, Piece{Color::Black, PieceType::Queen}, Piece{Color::Nil, PieceType::Nil}, Piece{Color::Black, PieceType::King}, Piece{Color::Black, PieceType::Knight}, Piece{Color::Black, PieceType::Rook}
    },
    game.castle = std::array<char, 4>{'K', '.', '.', '.'};
    game.en_passant = -1;

    TEST_ASSERT_EQUAL_CHESS_GAME(game, extract_make_move.game);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_make_move.past_move_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 230, 0), extract_make_move.lifted_square_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 255, 0), extract_make_move.legal_moves_color);
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(255, 0, 0), extract_make_move.illegal_moves_color);
    TEST_ASSERT_EQUAL(52, extract_make_move.past_move_from);
    TEST_ASSERT_EQUAL(36, extract_make_move.past_move_to);
    TEST_ASSERT_EQUAL(60000, extract_make_move.timeout);
}


void test_too_many_castling()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K...Q",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, castling must be 4 characters long", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_not_enough_castling()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "..k",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, castling must be 4 characters long", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_misplaced_castling_character()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K.Q.",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid castling character", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_castling_character()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K..A",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid castling character", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_en_passant()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K..q",
        "en_passant": not_en_passant,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, en passant must be a valid integer", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_number_en_passant()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K..q",
        "en_passant": 69,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid en passant number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_color()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K..q",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "a",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid player color", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_too_many_squares_in_board()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q....B.P.......p...........pppp.ppprnbq.knr....r...P...Q"
        "castling": "K..q",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, board must be 64 characters long", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_not_enough_squares_in_board()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NB.K.NR.PPP.PPPR....Q"
        "castling": "K..q",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, board must be 64 characters long", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_piece()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "make_move",
        "board": ".NA.K.PR.PPP.PPPC....Q....B.P.......p...........pppp.ppprnbq.knr"
        "castling": "K..q",
        "en_passant": -1,
        "past_move": {
            "color": [0, 0, 255],
            "from": 52,
            "to": 36
        },
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
        "lifted_square_color": [0, 230, 0],
        "legal_move_color": [0, 255, 0],
        "illegal_move_color": [255, 0, 0],
        "timeout_s": 60
    }
    )";
    try
    {
        auto _ = extractMakeMove(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid piece", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_functional_make_move);
    RUN_TEST(test_functional_en_passant_make_move);
    RUN_TEST(test_functional_castling_make_move);
    RUN_TEST(test_too_many_castling);
    RUN_TEST(test_not_enough_castling);
    RUN_TEST(test_misplaced_castling_character);
    RUN_TEST(test_invalid_castling_character);
    RUN_TEST(test_invalid_en_passant);
    RUN_TEST(test_invalid_number_en_passant);
    RUN_TEST(test_invalid_color);
    RUN_TEST(test_too_many_squares_in_board);
    RUN_TEST(test_not_enough_squares_in_board);
    RUN_TEST(test_invalid_piece);

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
