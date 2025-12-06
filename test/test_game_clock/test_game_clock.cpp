// test_game_clock.cpp
#include <unity.h>
#include "game_clock/game_clock.hpp"
#include "game_clock/game_clock.cpp"


void test_inactive_tick()
{
    ClockSetting c_settings(10, 5);
    c_settings.active.store(false);
    ClockColor previous_it = ClockColor::White;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(60000, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(60000, c_settings.time_black.load());
}


void test_white_tick()
{
    ClockSetting c_settings(10, 5);
    c_settings.player_turn = ClockColor::White;
    ClockColor previous_it = ClockColor::White;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(59999, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(60000, c_settings.time_black.load());
}


void test_black_tick()
{
    ClockSetting c_settings(10, 5);
    c_settings.player_turn = ClockColor::Black;
    ClockColor previous_it = ClockColor::Black;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(60000, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(59999, c_settings.time_black.load());
}


void test_white_extra_time()
{
    ClockSetting c_settings(10, 5);
    c_settings.player_turn = ClockColor::Black;
    ClockColor previous_it = ClockColor::White;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(60500, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(59999, c_settings.time_black.load());
}


void test_black_extra_time()
{
    ClockSetting c_settings(10, 5);
    c_settings.player_turn = ClockColor::White;
    ClockColor previous_it = ClockColor::Black;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(59999, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(60500, c_settings.time_black.load());
}


void test_countdown()
{
    ClockSetting c_settings(10, 5);
    c_settings.active.store(true);
    c_settings.time_white.store(1);
    c_settings.player_turn = ClockColor::White;
    ClockColor previous_it = ClockColor::White;
    tick(std::ref(c_settings), previous_it);

    TEST_ASSERT_EQUAL(0, c_settings.time_white.load());
    TEST_ASSERT_EQUAL(false, c_settings.active.load());
}


void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_inactive_tick);
    RUN_TEST(test_white_tick);
    RUN_TEST(test_black_tick);
    RUN_TEST(test_white_extra_time);
    RUN_TEST(test_black_extra_time);
    RUN_TEST(test_countdown);

    UNITY_END();
}

void loop() {}
