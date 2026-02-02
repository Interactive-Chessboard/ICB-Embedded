// test_animations.cpp

// test_game_clock.cpp
#include <unity.h>
#include <future>
#include "../test_helper.hpp"
#include "animations/animations.hpp"


void test_animations()
{
    Animation anim1;
    anim1.leds.at(61) = LedColor(255, 255, 255);
    anim1.display_time_ms = 1;
    Animation anim2;
    anim2.leds.at(40) = LedColor(25, 55, 4);
    anim2.display_time_ms = 1;
    std::vector<Animation> animations = {anim1, anim2};

    std::atomic<bool> stop_animations{false};
    std::string response = playAnimations(stop_animations, animations);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue = {anim1.leds, anim2.leds};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("Ok", response.c_str());
}


void test_animation_time_0()
{
    Animation anim1;
    anim1.leds.at(61) = LedColor(255, 255, 255);
    anim1.display_time_ms = 0;
    Animation anim2;
    anim2.leds.at(40) = LedColor(25, 55, 4);
    anim2.display_time_ms = 1;
    std::vector<Animation> animations = {anim1, anim2};

    std::atomic<bool> stop_animations{false};
    std::string response = playAnimations(stop_animations, animations);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue;
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("Error, display time must be greater than 0", response.c_str());
}



void test_animation_stop()
{
    Animation anim1;
    anim1.leds.at(61) = LedColor(255, 255, 255);
    anim1.display_time_ms = 100;
    Animation anim2;
    anim2.leds.at(40) = LedColor(25, 55, 4);
    anim2.display_time_ms = 1;
    std::vector<Animation> animations = {anim1, anim2};

    std::atomic<bool> stop_animations{false};
    auto future = std::async(std::launch::async, playAnimations, std::ref(stop_animations), animations);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    stop_animations.store(true);
    std::string response = future.get();

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue = {anim1.leds};
    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
    TEST_ASSERT_EQUAL_STRING("Error, stopped", response.c_str());
}


std::vector<std::array<LedColor, 64>> winner_animation_helper()
{
    std::vector<std::array<LedColor, 64>> led_queue;
    for (int i = 0; i < 2; i++)
    {
        std::array<LedColor, 64> arr;
        for (int j = 0; j < arr.size(); j++)
        {
            if (j % 2 == 0 && i % 2 == 0 || j % 2 != 0 && i % 2 != 0)
                arr.at(j) = LedColor(0, 0, 255);
            else
                arr.at(j) = LedColor(0, 0, 0);
        }
        led_queue.push_back(arr);
    }
    return led_queue;
}


void test_winner_animations_white_win()
{
    ChessGame game;
    game.winner = Winner::White;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue = winner_animation_helper();
    expected_led_queue.at(0).at(4) = LedColor(0, 255, 0);
    expected_led_queue.at(1).at(4) = LedColor(0, 255, 0);
    expected_led_queue.at(0).at(60) = LedColor(255, 0, 0);
    expected_led_queue.at(1).at(60) = LedColor(255, 0, 0);

    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_winner_animations_black_win()
{
    ChessGame game;
    game.winner = Winner::Black;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue = winner_animation_helper();
    expected_led_queue.at(0).at(4) = LedColor(255, 0, 0);
    expected_led_queue.at(1).at(4) = LedColor(255, 0, 0);
    expected_led_queue.at(0).at(60) = LedColor(0, 255, 0);
    expected_led_queue.at(1).at(60) = LedColor(0, 255, 0);

    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_winner_animations_draw()
{
    ChessGame game;
    game.winner = Winner::Draw;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    std::vector<std::array<LedColor, 64>> expected_led_queue = winner_animation_helper();

    TEST_ASSERT_LED_QUEUE(expected_led_queue, hardware_mock.set_led_queue);
}


void test_winner_animations_nil()
{
    ChessGame game;
    game.winner = Winner::Nil;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT(hardware_mock.set_led_queue.empty());
}


void test_winner_animations_no_white_king()
{
    ChessGame game;
    game.board.at(4) = Piece();
    game.winner = Winner::Nil;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT(hardware_mock.set_led_queue.empty());
}


void test_winner_animations_no_black_king()
{
    ChessGame game;
    game.board.at(4) = Piece();
    game.winner = Winner::Nil;
    playWinnerAnimation(game, 2, 1);

    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());
    TEST_ASSERT(hardware_mock.set_led_queue.empty());
}


void test_start_up_animation()
{
    playStartUpAnimation(1);
    MockHardware& hardware_mock = static_cast<MockHardware&>(Hardware::get());

    const std::array<LedColor, 64> ring1 = {
        LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255)
    };

    const std::array<LedColor, 64> ring2 = {
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    };

    const std::array<LedColor, 64> ring3 = {
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    };


    const std::array<LedColor, 64> ring4 = {
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    };
    std::vector<std::array<LedColor, 64>> expected_led_queue = {ring1, ring2, ring3, ring4, ring3, ring2, ring1};

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

    RUN_TEST(test_animations);
    RUN_TEST(test_animation_time_0);
    RUN_TEST(test_animation_stop);
    RUN_TEST(test_winner_animations_white_win);
    RUN_TEST(test_winner_animations_black_win);
    RUN_TEST(test_winner_animations_draw);
    RUN_TEST(test_winner_animations_nil);
    RUN_TEST(test_winner_animations_no_white_king);
    RUN_TEST(test_winner_animations_no_black_king);
    RUN_TEST(test_start_up_animation);

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
