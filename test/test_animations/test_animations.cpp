#include <unity.h>
#include <string>
#include <stdexcept>
#include "board.hpp"
#include "online_game/animations.hpp"
#include "online_game/extract_value.hpp"
#include "online_game/animations.cpp"


void test_functional_animation()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0]
        ],
        "time_ms": 1000
    },
    {
        "board": [
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0], [255,0,0],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255],
            [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255], [0,0,255]
        ],
        "time_ms": 800
    }
    ]
    )";
    std::vector<Animation> animations = parseAnimations(input);
    TEST_ASSERT_EQUAL(2, animations.size());

    Animation anim0 = animations[0];
    TEST_ASSERT_EQUAL(1000, anim0.display_time_ms);

    for (int i = 0; i < 32; i++) {
        LedColor led = anim0.leds[i];
        TEST_ASSERT_EQUAL(0, led.red);
        TEST_ASSERT_EQUAL(0, led.green);
        TEST_ASSERT_EQUAL(255, led.blue);  
    }
    for (int i = 32; i < 64; i++) {
        LedColor led = anim0.leds[i];
        TEST_ASSERT_EQUAL(255, led.red);
        TEST_ASSERT_EQUAL(0, led.green);
        TEST_ASSERT_EQUAL(0, led.blue);  
    }

    Animation anim1 = animations[1];
    TEST_ASSERT_EQUAL(800, anim1.display_time_ms);

    for (int i = 0; i < 32; i++) {
        LedColor led = anim1.leds[i];
        TEST_ASSERT_EQUAL(255, led.red);
        TEST_ASSERT_EQUAL(0, led.green);
        TEST_ASSERT_EQUAL(0, led.blue);  
    }
    for (int i = 32; i < 64; i++) {
        LedColor led = anim1.leds[i];
        TEST_ASSERT_EQUAL(0, led.red);
        TEST_ASSERT_EQUAL(0, led.green);
        TEST_ASSERT_EQUAL(255, led.blue);  
    }
}


void test_light_red_above_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [256,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors must be lower than 256", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_green_above_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,256,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors must be lower than 256", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_blue_above_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,256], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors must be lower than 256", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_red_under_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [-1,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_green_under_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,-1,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_blue_under_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,-1], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_timeout_under_bounds()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": -1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception.");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_red_non_number()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [a,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be a number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_green_non_number()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,dnjh&,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be a number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_light_blue_non_number()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,;,0]
        ],
        "time_ms": 1000
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be a number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_timeout_non_number()
{
    std::string input = R"(
    [
    {
        "board": [
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0],
            [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0], [0,0,0]
        ],
        "time_ms": sjjdf
    }
    ]
    )";

    try
    {
        auto animations = parseAnimations(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, colors or timeout must be a number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_functional_animation);
    RUN_TEST(test_light_red_above_bounds);
    RUN_TEST(test_light_green_above_bounds);
    RUN_TEST(test_light_blue_above_bounds);
    RUN_TEST(test_light_red_under_bounds);
    RUN_TEST(test_light_green_under_bounds);
    RUN_TEST(test_light_blue_under_bounds);
    RUN_TEST(test_timeout_under_bounds);
    RUN_TEST(test_light_red_non_number);
    RUN_TEST(test_light_green_non_number);
    RUN_TEST(test_light_blue_non_number);
    RUN_TEST(test_timeout_non_number);

    UNITY_END();
}

void loop() {}
