// test_set_board
#include <unity.h>
#include "../test_helper.hpp"
#include "set_board/set_board.hpp"


void test_set_board_offline()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    int past_move_from = 12;
    int past_move_to = 28;
    SetBoard set_board(game, past_move_from, past_move_to);

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
    ChessGame game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPpPp/P1NP3P/1B1Q2B1/R3K2R w KQkq - 0 1");
    int past_move_from = 36;
    int past_move_to = 27;
    SetBoard set_board(game, past_move_from, past_move_to);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b17f009463b9ULL
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(27) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(27) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_offline_stop()
{
    ChessGame game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPpPp/P1NP3P/1B1Q2B1/R3K2R w KQkq - 0 1");
    int past_move_from = 36;
    int past_move_to = 27;
    SetBoard set_board(game, past_move_from, past_move_to);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b17f009463b9ULL
    };

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &SetBoard::startOffline, &set_board, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    future.get();

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(0, 0, 255);
    leds1.at(27) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_online()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    extract_set_board.past_move_from = 12;
    extract_set_board.past_move_to = 28;
    extract_set_board.past_move_color = LedColor(20, 20, 200);
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
    extract_set_board.game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPpPp/P1NP3P/1B1Q2B1/R3K2R w KQkq - 0 1");
    extract_set_board.past_move_from = 36;
    extract_set_board.past_move_to = 27;
    extract_set_board.past_move_color = LedColor(20, 20, 200);
    extract_set_board.timeout = 100000;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b16f009463b9ULL,
        0x8952b17f009463b9ULL
    };

    std::atomic<bool> active{true};
    std::string response = set_board.startOnline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(20, 20, 200);
    leds1.at(27) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds2;
    leds2.at(27) = LedColor(20, 20, 200);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("ok", response.c_str());
}


void test_set_board_online_stop()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPpPp/P1NP3P/1B1Q2B1/R3K2R w KQkq - 0 1");
    extract_set_board.past_move_from = 36;
    extract_set_board.past_move_to = 27;
    extract_set_board.past_move_color = LedColor(20, 20, 40);
    extract_set_board.timeout = 100000;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b17f009463b9ULL
    };

    std::atomic<bool> active{true};
    auto future = std::async(std::launch::async, &SetBoard::startOnline, &set_board, std::ref(active));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    active.store(false);
    std::string response = future.get();

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(20, 20, 40);
    leds1.at(27) = LedColor(20, 20, 40);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", response.c_str());
}


void test_set_board_online_timeout()
{
    ExtractSetBoard extract_set_board;
    extract_set_board.game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPpPp/P1NP3P/1B1Q2B1/R3K2R w KQkq - 0 1");
    extract_set_board.past_move_from = 36;
    extract_set_board.past_move_to = 27;
    extract_set_board.past_move_color = LedColor(12, 67, 203);
    extract_set_board.timeout = 2;

    SetBoard set_board(extract_set_board);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
        0x8952b16f089463b9ULL,
    };

    std::atomic<bool> active{true};
    std::string response = set_board.startOnline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(36) = LedColor(12, 67, 203);
    leds1.at(27) = LedColor(12, 67, 203);

    std::array<LedColor, 64> leds2;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("error, timeout reached or end task called", response.c_str());
}


void test_set_board_wrong_move()
{
    ChessGame game = chessgameFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    int past_move_from = 12;
    int past_move_to = 28;
    SetBoard set_board(game, past_move_from, past_move_to);

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


void test_set_board_test_starting_pos()
{
    ChessGame game;
    int past_move_from = -1;
    int past_move_to = -1;
    SetBoard set_board(game, past_move_from, past_move_to);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0xfff700080000ffffULL,
        0xfff700000000ffffULL,
        0xffff00000000ffffULL,
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(12) = LedColor(0, 0, 255);
    leds1.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(12) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_capture()
{
    ChessGame game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPQPp/P1NP3P/1B4B1/R3K2R b KQkq - 0 1");
    int past_move_from = 11;
    int past_move_to = 29;
    SetBoard set_board(game, past_move_from, past_move_to);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8942b17f009463b9ULL,
        0x8942b17b009463b9ULL,
        0x8942b17f009463b9ULL,
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(11) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds3;

    std::vector<std::array<LedColor, 64>> expected_led_queue = {leds1, leds2, leds3};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_set_board_capture_lift_capture_first()
{
    ChessGame game = chessgameFromFen("r1bqk2r/1pp3pn/p2p1p2/8/1PPbPQPp/P1NP3P/1B4B1/R3K2R b KQkq - 0 1");
    int past_move_from = 11;
    int past_move_to = 29;
    SetBoard set_board(game, past_move_from, past_move_to);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    hardware_mock.get_board_arr_queue = {
        0x8952b17f009463b9ULL,
        0x8952b17b009463b9ULL,
        0x8942b17b009463b9ULL,
        0x8942b17f009463b9ULL,
    };

    std::atomic<bool> active{true};
    set_board.startOffline(active);

    std::array<LedColor, 64> leds1;
    leds1.at(11) = LedColor(0, 0, 255);
    leds1.at(29) = LedColor(0, 0, 255);

    std::array<LedColor, 64> leds2;
    leds2.at(29) = LedColor(0, 0, 255);

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

    RUN_TEST(test_set_board_offline);
    RUN_TEST(test_set_board_offline_extra_delay);
    RUN_TEST(test_set_board_offline_stop);
    RUN_TEST(test_set_board_online);
    RUN_TEST(test_set_board_online_extra_delay);
    RUN_TEST(test_set_board_online_stop);
    RUN_TEST(test_set_board_online_timeout);
    RUN_TEST(test_set_board_wrong_move);
    RUN_TEST(test_set_board_test_starting_pos);
    RUN_TEST(test_set_board_capture);
    RUN_TEST(test_set_board_capture_lift_capture_first);

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
