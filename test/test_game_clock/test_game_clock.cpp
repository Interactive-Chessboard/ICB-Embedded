// test_game_clock.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "game_clock/game_clock.hpp"


void test_inactive_clock()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};

    clock_settings.active.store(false);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_EQUAL(600000, clock_settings.time_white.load());
    TEST_ASSERT_EQUAL(600000, clock_settings.time_black.load());
}


void test_white_clock()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_CLOCK_TOLERANCE(599995, clock_settings.time_white.load());
    TEST_ASSERT_EQUAL(600000, clock_settings.time_black.load());
}


void test_black_clock()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.player_turn.store(Color::Black);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_EQUAL(600000, clock_settings.time_white.load());
    TEST_ASSERT_CLOCK_TOLERANCE(599995, clock_settings.time_black.load());
}


void test_white_extra_time()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    clock_settings.player_turn = Color::Black;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_CLOCK_TOLERANCE(604995, clock_settings.time_white.load());
    TEST_ASSERT_CLOCK_TOLERANCE(599995, clock_settings.time_black.load());
}


void test_black_extra_time()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.player_turn.store(Color::Black);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    clock_settings.player_turn = Color::White;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_CLOCK_TOLERANCE(599995, clock_settings.time_white.load());
    TEST_ASSERT_CLOCK_TOLERANCE(604995, clock_settings.time_black.load());
}


void test_white_time_reaches_0()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.time_white.store(1);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_EQUAL(0, clock_settings.time_white.load());
    TEST_ASSERT_EQUAL(false, clock_settings.active.load());

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT_EQUAL_STRING("White Time: 0.0", hardware_mock.time_screen_calls.at(0).at(0).c_str());
    TEST_ASSERT_EQUAL_STRING("Black Time: 10:00.0", hardware_mock.time_screen_calls.at(0).at(1).c_str());
}


void test_black_time_reaches_0()
{
    ClockSetting clock_settings(10, 5);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.player_turn.store(Color::Black);
    clock_settings.time_black.store(1);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    TEST_ASSERT_EQUAL(0, clock_settings.time_black.load());
    TEST_ASSERT_EQUAL(false, clock_settings.active.load());

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT_EQUAL_STRING("White Time: 10:00.0", hardware_mock.time_screen_calls.at(0).at(0).c_str());
    TEST_ASSERT_EQUAL_STRING("Black Time: 0.0", hardware_mock.time_screen_calls.at(0).at(1).c_str());
}


void test_time_white_in_hours()
{
    ClockSetting clock_settings(100, 100);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.time_white.fetch_sub(8);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT_EQUAL_STRING("White Time: 1:39:59.9", hardware_mock.time_screen_calls.at(0).at(0).c_str());
    TEST_ASSERT_EQUAL_STRING("Black Time: 1:40:00.0", hardware_mock.time_screen_calls.at(0).at(1).c_str());
}


void test_time_black_in_hours()
{
    ClockSetting clock_settings(160, 160);
    std::atomic<bool> stop_clock_thread{false};
    clock_settings.player_turn.store(Color::Black);
    clock_settings.time_black.fetch_sub(8);

    std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_clock_thread.store(true);
    clock_thread.join();

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT_EQUAL_STRING("White Time: 2:40:00.0", hardware_mock.time_screen_calls.at(0).at(0).c_str());
    TEST_ASSERT_EQUAL_STRING("Black Time: 2:39:59.9", hardware_mock.time_screen_calls.at(0).at(1).c_str());
}


// runs before each test
void setUp()
{
    setUpMockHardware();
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_inactive_clock);
    RUN_TEST(test_white_clock);
    RUN_TEST(test_black_clock);
    RUN_TEST(test_white_extra_time);
    RUN_TEST(test_black_extra_time);
    RUN_TEST(test_white_time_reaches_0);
    RUN_TEST(test_time_white_in_hours);
    RUN_TEST(test_time_black_in_hours);

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
