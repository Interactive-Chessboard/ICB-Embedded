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



// MockHardware
class MockHardware : public IHardware
{
private:
    MockHardware() = default;
public:
    static MockHardware& instance()
    {
        static MockHardware instance;
        return instance;
    }

    MockHardware(const MockHardware&) = delete;
    MockHardware& operator=(const MockHardware&) = delete;

    // bluetoothInit mock does nothing
    void bluetoothInit() override {}

    // std::deque for getBluetoothMessage
    std::deque<std::string> get_bluetooth_messages_queue;
    // getBluetoothMessage mock returns messages from a std::deque
    std::string getBluetoothMessage() override
    {
        std::string msg;
        if (!get_bluetooth_messages_queue.empty())
        {
            msg = get_bluetooth_messages_queue.front();
            get_bluetooth_messages_queue.pop_front();
        }
        return msg;
    }

    // vector for sendBluetoothMessage
    std::vector<std::string> send_bluetooth_messages_queue;
    // sendBluetoothMessage mock saves messages to a vector
    void sendBluetoothMessage(std::string msg) override
    {
        send_bluetooth_messages_queue.push_back(msg);
    }

    // std::deque for getBoardArr
    std::deque<uint64_t> get_board_arr_queue;
    // getBoardArr mock returns messages from a std::deque
    uint64_t getBoardArr() override
    {
        uint64_t val;
        if (!get_board_arr_queue.empty())
        {
            val = get_board_arr_queue.front();
            get_board_arr_queue.pop_front();
        }
        return val;
    }

    // vector for setLed
    std::vector<std::array<LedColor, 64>> set_led_queue;
    // ssetLed mock saves messages to a vector
    void setLed(std::array<LedColor, 64> led) override
    {
        set_led_queue.push_back(led);
    }

    // clearLed mock does nothing
    void clearLed() override {}

    void reserveScreen(bool) override;
    void setTimeScreen(std::vector<std::string>) override;
    void setScreen(std::vector<std::string>, int) override;
    bool detectSelectClick() override;
    bool detectStartClick() override;
};