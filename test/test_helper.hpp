// test_helper.hpp
#include <deque>
#include <string>
#include <vector>
#include <future>
#include <sstream>
#include <array>
#include <cctype>
#include "unity.h"
#include "hardware.hpp"
#include "chess.hpp"


inline void TEST_ASSERT_EQUAL_LED_COLOR(const LedColor &expected, const LedColor &actual)
{
    TEST_ASSERT_EQUAL(expected.red, actual.red);
    TEST_ASSERT_EQUAL(expected.green, actual.green);
    TEST_ASSERT_EQUAL(expected.blue, actual.blue);
}


inline void TEST_ASSERT_EQUAL_CHESS_GAME(const ChessGame &expected, const ChessGame &actual)
{
    TEST_ASSERT_EQUAL(expected.player_turn, actual.player_turn);
    TEST_ASSERT_EQUAL(expected.board.size(), actual.board.size());
    for (int i = 0; i < expected.board.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.board.at(i).color, actual.board.at(i).color);
        TEST_ASSERT_EQUAL(expected.board.at(i).piece_type, actual.board.at(i).piece_type);
    }
    TEST_ASSERT_EQUAL(expected.castle.size(), actual.castle.size());
    for (int i = 0; i < expected.castle.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.castle.at(i), actual.castle.at(i));
    }
    TEST_ASSERT_EQUAL(expected.en_passant, actual.en_passant);
}


inline void TEST_ASSERT_UINT64_T(const uint64_t &expected, const uint64_t &actual)
{
    TEST_ASSERT_EQUAL_MEMORY(expected, actual, sizeof(uint64_t));
}


inline void TEST_ASSERT_CLOCK_TOLERANCE(const int &expected, const int &actual)
{
    TEST_ASSERT_GREATER_OR_EQUAL(expected - 3, actual);
    TEST_ASSERT_LESS_OR_EQUAL(expected + 3, actual);
}

inline void TEST_ASSERT_LED_QUEUE(const std::vector<std::array<LedColor, 64>> &expected, const std::vector<std::array<LedColor, 64>> &actual)
{
    TEST_ASSERT_EQUAL(expected.size(), actual.size());
    for (int i = 0; i < expected.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.at(i).size(), actual.at(i).size());
        for (int j = 0; j < expected.at(i).size(); j++)
        {
            TEST_ASSERT_EQUAL_LED_COLOR(expected.at(i).at(j), actual.at(i).at(j));
        }
    }
}


class MockHardware : public IHardware
{
private:
    MockHardware() = default;
    void handleOtaWrite(const std::string&) override {};

public:
    static MockHardware& instance()
    {
        static MockHardware instance;
        return instance;
    }

    void reset()
    {
        get_bluetooth_messages_queue.clear();
        send_bluetooth_messages_queue.clear();
        get_board_arr_queue.clear();
        set_led_queue.clear();
        reserve_screen_queue.clear();
        time_screen_calls.clear();
        screen_calls.clear();
        select_click_queue.clear();
        start_click_queue.clear();
    }

    MockHardware(const MockHardware&) = delete;
    MockHardware& operator=(const MockHardware&) = delete;

    // ---------------- Bluetooth ----------------
    std::string getFirmwareVersion() override
    {
        return "v1.2.3";
    }

    std::string getMacAddress() override
    {
        return "A4:CF:12:9B:00:12";
    }

    // ---------------- Bluetooth ----------------

    void bluetoothInit() override {}

    std::deque<std::string> get_bluetooth_messages_queue;
    std::string getBluetoothMessage() override
    {
        if (get_bluetooth_messages_queue.empty())
            return {};

        std::string msg = get_bluetooth_messages_queue.front();
        get_bluetooth_messages_queue.pop_front();
        return msg;
    }

    std::vector<std::string> send_bluetooth_messages_queue;
    void sendBluetoothMessage(const std::string& msg) override
    {
        send_bluetooth_messages_queue.push_back(std::move(msg));
    }

    // ---------------- Board ----------------

    std::deque<uint64_t> get_board_arr_queue;
    uint64_t getBoardArr() override
    {
        if (get_board_arr_queue.empty())
            return 0;

        uint64_t val = get_board_arr_queue.front();
        get_board_arr_queue.pop_front();
        return val;
    }

    // ---------------- LEDs ----------------

    std::vector<std::array<LedColor, 64>> set_led_queue;
    void setLed(const std::array<LedColor, 64>& leds) override
    {
        bool skip = true;
        if (set_led_queue.size() <= 0)
            set_led_queue.push_back(leds);
        else
        {
            bool all_equal = true;
            std::array<LedColor, 64> old_leds = set_led_queue.back();
            for (int i = 0; i < leds.size(); i++)
            {
                if (!(leds.at(i) == old_leds.at(i)))
                    all_equal = false;
            }
            if (!all_equal) // Leds equivalent to the previous are not added to the list
                set_led_queue.push_back(leds);
        }
    }

    void clearLed() override
    {
        std::array<LedColor, 64> leds;
        setLed(leds);
    }

    // ---------------- Screen ----------------

    std::vector<bool> reserve_screen_queue;
    void reserveScreen(bool reserve) override
    {
        reserve_screen_queue.push_back(reserve);
    }

    std::vector<std::vector<std::string>> time_screen_calls;
    void setTimeScreen(const std::vector<std::string>& text) override
    {
        time_screen_calls.push_back(std::move(text));
    }

    struct ScreenCall {
        std::vector<std::string> text;
        int selected;
    };
    std::vector<ScreenCall> screen_calls;
    void setScreen(const std::vector<std::string>& text, int selected) override
    {
        screen_calls.push_back({ std::move(text), selected });
    }

    void clearScreen() override {}

    // ---------------- Buttons ----------------

    std::deque<bool> select_click_queue;
    bool detectSelectClick() override
    {
        if (select_click_queue.empty())
            return false;

        bool val = select_click_queue.front();
        select_click_queue.pop_front();
        return val;
    }

    std::deque<bool> start_click_queue;
    bool detectStartClick() override
    {
        if (start_click_queue.empty())
            return false;

        bool val = start_click_queue.front();
        start_click_queue.pop_front();
        return val;
    }
};


// Run in setUp() function to run before every test
void setUpMockHardware()
{
    static MockHardware& mock_hardware = MockHardware::instance();
    mock_hardware.reset();
    Hardware::set(mock_hardware);
}


ChessGame chessgameFromFen(const std::string& fen)
{
    ChessGame game; // default = starting position

    std::istringstream iss(fen);

    std::string board_part, turn_part, castle_part, ep_part;
    int halfmove, fullmove;

    // FEN must have at least 4 fields
    if (!(iss >> board_part >> turn_part >> castle_part >> ep_part))
        return game;

    // Optional halfmove/fullmove — ignore if missing
    iss >> halfmove >> fullmove;

    // -------- Parse board --------
    std::array<Piece, 64> new_board{};
    int index = 56; // start at rank 8 (a8)

    for (char c : board_part)
    {
        if (c == '/')
        {
            index -= 16; // move down one rank
            continue;
        }

        if (std::isdigit(c))
        {
            int empty = c - '0';
            for (int i = 0; i < empty; ++i)
            {
                if (index < 0 || index >= 64) return game;
                new_board[index++] = Piece();
            }
        }
        else
        {
            if (index < 0 || index >= 64) return game;

            Color color = std::isupper(c) ? Color::White : Color::Black;
            char lower = std::tolower(c);

            PieceType type;

            switch (lower)
            {
                case 'p': type = PieceType::Pawn; break;
                case 'r': type = PieceType::Rook; break;
                case 'n': type = PieceType::Knight; break;
                case 'b': type = PieceType::Bishop; break;
                case 'q': type = PieceType::Queen; break;
                case 'k': type = PieceType::King; break;
                default: return game; // invalid char
            }

            new_board[index++] = Piece(color, type);
        }
    }

    if (index != 8) return game; // must end exactly at h1

    // -------- Parse turn --------
    if (turn_part == "w")
        game.player_turn = Color::White;
    else if (turn_part == "b")
        game.player_turn = Color::Black;
    else
        return game;

    // -------- Parse castling --------
    game.castle = {'.', '.', '.', '.'};

    if (castle_part != "-")
    {
        for (char c : castle_part)
        {
            switch (c)
            {
                case 'K': game.castle[0] = 'K'; break;
                case 'Q': game.castle[1] = 'Q'; break;
                case 'k': game.castle[2] = 'k'; break;
                case 'q': game.castle[3] = 'q'; break;
                default: return game;
            }
        }
    }

    // -------- Parse en passant --------
    if (ep_part == "-")
    {
        game.en_passant = -1;
    }
    else
    {
        if (ep_part.size() != 2) return game;

        char file = ep_part[0];
        char rank = ep_part[1];

        if (file < 'a' || file > 'h') return game;
        if (rank < '1' || rank > '8') return game;

        int file_index = file - 'a';
        int rank_index = rank - '1';

        game.en_passant = rank_index * 8 + file_index;
    }

    // If we reached here, board is valid
    game.board = new_board;
    game.winner = Chess::calculateEndGame(game, Chess::generateLegalMoves(game));

    return game;
}
