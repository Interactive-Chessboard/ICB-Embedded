#include <unity.h>
#include <string>
#include "online_game/animations.hpp"
#include "board.hpp"
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

// Function you want to test
int add(int a, int b) {
    return a + b;
}


void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_functional_animation);

    UNITY_END();
}

void loop() {}
