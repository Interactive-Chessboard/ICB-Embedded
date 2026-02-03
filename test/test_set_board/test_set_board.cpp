// test_animations.cpp

// test_game_clock.cpp
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

    std::array<LedColor, 64> led1;
    led1.at(12) = LedColor(0, 0, 255);
    led1.at(28) = LedColor(0, 0, 255);

    std::array<LedColor, 64> led2;
    led2.at(28) = LedColor(0, 0, 255);

    std::vector<std::array<LedColor, 64>> expected_led_queue = {led1, led2};
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
