// test_extract_value.cpp
#include <unity.h>
#include "online_game/extract_value.hpp"
#include "online_game/extract_value.cpp"

void test_extract_string()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "old_move": {
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "started": "true",
            "white": 50,
            "black": 60,
            "run_down": "w",
        },
        "timeout": 60
    }
    )";

    std::string value;
    try
    {
        value = extract_value(input, "type");
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
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "started": "true",
            "white": 50,
            "black": 60,
            "run_down": "w",
        },
        "timeout": 60
    }
    )";

    std::string value;
    try
    {
        value = extract_value(input, "id");
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
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "started": "true",
            "white": 50,
            "black": 60,
            "run_down": "w",
        },
        "timeout": 60
    }
    )";

    std::string value;
    try
    {
        value = extract_value(input, "clock");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    std::string expected = R"({
            "started": "true",
            "white": 50,
            "black": 60,
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
            "color": [0, 0, 255],
            "from": 12,
            "to": 28
        },
        "clock": {
            "started": "true",
            "white": 50,
            "black": 60,
            "run_down": "w",
        },
        "timeout": 60
    }
    )";

    std::string value;
    try
    {
        value = extract_value(extract_value(input, "clock"), "run_down");
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_STRING("w", value.c_str());
}


void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_extract_string);
    RUN_TEST(test_extract_int);
    RUN_TEST(test_extract_json);
    RUN_TEST(test_extract_nested);

    UNITY_END();
}

void loop() {}