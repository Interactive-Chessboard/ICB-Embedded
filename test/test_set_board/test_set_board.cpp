// test_set_board
#include <unity.h>
#include "../test_helper.hpp"
#include "set_board/set_board.hpp"


void test_set_board_offline()
{
    SetBoard set_board(0xfff700080000ffffULL);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL,
        0xfff700000000ffffULL,
        0xfff700080000ffffULL
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(0, 0, 255);
    leds1.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_offline_extra_delay()
{
    SetBoard set_board(0x2002094b82ee4200ULL);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x2002094b82ee4200ULL
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(2) = LedColor(0, 0, 255);
    leds1.at(3) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(2) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_offline_stop()
{
    SetBoard set_board(0x2002094b82ee4200ULL);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL
    };

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &SetBoard::startOffline, &set_board, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    future.get();

    std::array<LedColor, 64> leds1;
    leds1.at(2) = LedColor(0, 0, 255);
    leds1.at(3) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_online()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.board = 0xfff700080000ffffULL;
    extract_set_board.color = LedColor(20, 20, 200);
    extract_set_board.timeout = 100000;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL,
        0xfff700000000ffffULL,
        0xfff700080000ffffULL
    };

    std::atomic<bool> active{true};
    std::string response = set_board.startOnline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(20, 20, 200);
    leds1.at(28) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds2;
    leds2.at(28) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("ok", response.c_str());
}


void test_set_board_online_extra_delay()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.board = 0x2002094b82ee4200ULL;
    extract_set_board.color = LedColor(20, 20, 200);
    extract_set_board.timeout = 100000;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x0002094b82ee4200ULL,
        0x2002094b82ee4200ULL
    };

    std::atomic<bool> active{true};
    std::string response = set_board.startOnline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(2) = LedColor(20, 20, 200);
    leds1.at(3) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds2;
    leds2.at(2) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("ok", response.c_str());
}


void test_set_board_online_stop()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.board = 0x2002094b82ee4200ULL;
    extract_set_board.color = LedColor(20, 20, 40);
    extract_set_board.timeout = 100000;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL
    };

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &SetBoard::startOnline, &set_board, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    std::string response = future.get();

    std::array<LedColor, 64> leds1;
    leds1.at(2) = LedColor(20, 20, 40);
    leds1.at(3) = LedColor(20, 20, 40);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", response.c_str());
}


void test_set_board_online_timeout()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.board = 0x2002094b82ee4200ULL;
    extract_set_board.color = LedColor(12, 67, 203);
    extract_set_board.timeout = 2;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL,
        0x1002094b82ee4200ULL
    };

    std::atomic<bool> active{true};
    std::string response = set_board.startOnline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(2) = LedColor(12, 67, 203);
    leds1.at(3) = LedColor(12, 67, 203);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", response.c_str());
}


void test_set_board_wrong_move()
{
    SetBoard set_board(0xfff700080000ffffULL);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xffff00000000ffffULL,
        0xfff700000000ffffULL,
        0x7ff700000000ffffULL,
        0xfff700000000ffffULL,
        0xfff700080000ffffULL
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(0, 0, 255);
    leds1.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;
    leds3.at(0) = LedColor(0, 0, 255);
    leds3.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds4;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3, leds2, leds4};
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

    RUN_TEST(test_set_board_offline);
    RUN_TEST(test_set_board_offline_extra_delay);
    RUN_TEST(test_set_board_offline_stop);
    RUN_TEST(test_set_board_online);
    RUN_TEST(test_set_board_online_extra_delay);
    RUN_TEST(test_set_board_online_stop);
    RUN_TEST(test_set_board_online_timeout);
    RUN_TEST(test_set_board_wrong_move);

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
