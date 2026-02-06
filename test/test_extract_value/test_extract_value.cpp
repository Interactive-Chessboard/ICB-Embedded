// test_extract_value.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "online_game/extract_value/extract_value.hpp"


void test_extract_string()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255]
        },
        "clock": {
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";

    std::string value;
    try
    {
        value = extractValue(input, "type");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_STRING("set_board", value.c_str());
}


void test_extract_int()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255]
        },
        "clock": {
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";

    std::string value;
    try
    {
        value = extractValue(input, "id");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL(1, stoi(value));
}


void test_extract_json()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255]
        },
        "clock": {
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";

    std::string value;
    try
    {
        value = extractValue(input, "clock");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    std::string expected = R"({
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        })";
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), value.c_str());
}


void test_extract_nested()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255]
        },
        "clock": {
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";

    std::string value;
    try
    {
        value = extractValue(extractValue(input, "clock"), "run_down");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_STRING("w", value.c_str());
}


void test_extract_key_not_found()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255]
        },
        "clock": {
            "started": "true",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";

    try
    {
        auto _ = extractValue(input, "invalid_key");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, key not found", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_extract_missing_semicolon()
{
    std::string input = R"(
    {
        "id": 1,
        "type" "set_board"
    }
    )";

    try
    {
        auto _ = extractValue(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, expecting semicolon after key", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_extract_missing_closing_quotes()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board
    }
    )";

    try
    {
        auto _ = extractValue(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, expecting closing quotes", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_extract_missing_closing_square_bracket()
{
    std::string input = R"(
    {
        "id": 1,
        "type": [1, 2, 3,
        "key": "val"
    }
    )";

    try
    {
        auto _ = extractValue(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, expecting square bracket", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_valid_timeout()
{
    std::string input = R"(
    {
        "id": 1,
        "timeout_s": 20
    }
    )";

    int value;
    try
    {
        value = extractTimeOut(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    // timeout result in centiseconds
    TEST_ASSERT_EQUAL(20000, value);
}


void test_timeout_not_number()
{
    std::string input = R"(
    {
        "id": 1,
        "timeout_s": aaa
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, timeout must be a valid number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_timeout_negative_number()
{
    std::string input = R"(
    {
        "id": 1,
        "timeout_s": -1
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, timeout must be a valid positive number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_timeout_overflow()
{
    std::string input = R"(
    {
        "id": 1,
        "timeout_s": 99999999999999999999999999999999999999999999999
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, timeout must be a valid number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_valid_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL(true, clock_setting.active.load());
    TEST_ASSERT_EQUAL(50000, clock_setting.time_white.load());
    TEST_ASSERT_EQUAL(60000, clock_setting.time_black.load());
    TEST_ASSERT_EQUAL(5000, clock_setting.extra_time.load());
    TEST_ASSERT_EQUAL(Color::White, clock_setting.player_turn.load());
}


void test_inactive_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "f",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    // Inactive makes it so the base values are not modified
    TEST_ASSERT_EQUAL(false, clock_setting.active.load());
    TEST_ASSERT_EQUAL(600000, clock_setting.time_white.load());
    TEST_ASSERT_EQUAL(600000, clock_setting.time_black.load());
    TEST_ASSERT_EQUAL(10000, clock_setting.extra_time.load());
    TEST_ASSERT_EQUAL(Color::White, clock_setting.player_turn.load());
}


void test_run_down_black_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL(true, clock_setting.active.load());
    TEST_ASSERT_EQUAL(50000, clock_setting.time_white.load());
    TEST_ASSERT_EQUAL(60000, clock_setting.time_black.load());
    TEST_ASSERT_EQUAL(5000, clock_setting.extra_time.load());
    TEST_ASSERT_EQUAL(Color::Black, clock_setting.player_turn.load());
}

void test_invalid_active_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "a",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, expecting true (t) or false (f)", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_white_time_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": aaa,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, time values must be valid", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_overflow_black_time_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 999999999999999999999999999999999999999999999999999999,
            "extra_time_ms": 5000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, time values must be valid", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_negative_extra_time_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 700,
            "extra_time_ms": -1000,
            "run_down": "b",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, time values must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_invalid_run_down_color_clock_settings()
{
    ClockSetting clock_setting(10, 10);
    std::string input = R"(
    {
        "id": 1,
        "clock": {
            "active": "t",
            "white_ms": 50000,
            "black_ms": 700,
            "extra_time_ms": 5000,
            "run_down": "not_color",
        },
    }
    )";

    try
    {
        setClockSettings(clock_setting, input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid run down clock color", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_valid_led_color()
{
    std::string input = "[255, 127, 3]";
    LedColor led_color;
    try
    {
        led_color = getLedColor(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(255, 127, 3), led_color);
}


void test_valid_too_many_colors()
{
    std::string input = "[55, 17, 35, 83, 23, 8]";
    LedColor led_color;
    try
    {
        led_color = getLedColor(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    // Function only grabs the first three
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(55, 17, 35), led_color);
}


void test_led_color_not_number()
{
    std::string input = "[255, 127, aaa]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid LED color format", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_fixed_string()
{
    std::string input = "[sfs55kfs, kal17fw, fj35pwk]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid LED color format", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_over_255()
{
    std::string input = "[256, 9, 2]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, colors must be lower than 256", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_negative()
{
    std::string input = "[255, -9, 2]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, colors must be positive", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_no_brakets()
{
    std::string input = "255, -9, 2";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid LED color format", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_no_comma()
{
    std::string input = "[255 -9 2]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid LED color format", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_led_color_not_enough_values()
{
    std::string input = "[255, -9]";
    try
    {
        auto _ = getLedColor(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("error, invalid LED color format", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void runTests()
{
    UNITY_BEGIN();

    // test extractValue()
    RUN_TEST(test_extract_string);
    RUN_TEST(test_extract_int);
    RUN_TEST(test_extract_json);
    RUN_TEST(test_extract_nested);
    RUN_TEST(test_extract_key_not_found);
    RUN_TEST(test_extract_missing_semicolon);
    RUN_TEST(test_extract_missing_closing_quotes);
    RUN_TEST(test_extract_missing_closing_square_bracket);

    // test extractTimeOut()
    RUN_TEST(test_valid_timeout);
    RUN_TEST(test_timeout_not_number);
    RUN_TEST(test_timeout_negative_number);
    RUN_TEST(test_timeout_overflow);

    // test setClockSettings
    RUN_TEST(test_valid_clock_settings);
    RUN_TEST(test_inactive_clock_settings);
    RUN_TEST(test_run_down_black_clock_settings);
    RUN_TEST(test_invalid_active_clock_settings);
    RUN_TEST(test_invalid_white_time_clock_settings);
    RUN_TEST(test_overflow_black_time_clock_settings);
    RUN_TEST(test_negative_extra_time_clock_settings);
    RUN_TEST(test_invalid_run_down_color_clock_settings);

    // test getLEDColor()
    RUN_TEST(test_valid_led_color);
    RUN_TEST(test_valid_too_many_colors);
    RUN_TEST(test_led_color_not_number);
    RUN_TEST(test_led_color_fixed_string);
    RUN_TEST(test_led_color_over_255);
    RUN_TEST(test_led_color_negative);
    RUN_TEST(test_led_color_no_brakets);
    RUN_TEST(test_led_color_no_comma);
    RUN_TEST(test_led_color_not_enough_values);

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
