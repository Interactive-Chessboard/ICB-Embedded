// test_helper.hpp
#include <deque>
#include <string>
#include <vector>
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
    TEST_ASSERT_GREATER_OR_EQUAL(expected - 2, actual);
    TEST_ASSERT_LESS_OR_EQUAL(expected + 2, actual);
}

#include <iostream>
inline void TEST_ASSERT_LED_QUEUE(const std::vector<std::array<LedColor, 64>> &expected, const std::vector<std::array<LedColor, 64>> &actual)
{
    TEST_ASSERT_EQUAL(expected.size(), actual.size());
    for (int i = 0; i < expected.size(); i++)
    {
        TEST_ASSERT_EQUAL(expected.at(i).size(), actual.at(i).size());
        for (int j = 0; j < expected.at(i).size(); j++)
        {
            //std::cout << int(expected.at(i).at(j).red) << " red " << int(actual.at(i).at(j).red) << "\n";
            //std::cout << int(expected.at(i).at(j).green) << " green " << int(actual.at(i).at(j).green) << "\n";
            //std::cout << int(expected.at(i).at(j).blue) <<  " blue " << int(actual.at(i).at(j).blue) << "\n";
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

    void clearLed() override {}

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
