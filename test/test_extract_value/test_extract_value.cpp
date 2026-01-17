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
            "color": [0, 0, 255]
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
            "color": [0, 0, 255]
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
            "color": [0, 0, 255]
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
            "color": [0, 0, 255]
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
            "white": 50,
            "black": 60,
            "run_down": "w",
        },
        "timeout": 60
    }
    )";

    try
    {
        auto _ = extract_value(input, "invalid_key");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, key not found", e.what());
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
        auto _ = extract_value(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, expecting semicolon after key", e.what());
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
        auto _ = extract_value(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, expecting closing quotes", e.what());
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
        auto _ = extract_value(input, "type");
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, expecting square bracket", e.what());
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
        "timeout": 20
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
    TEST_ASSERT_EQUAL(20, value);
}


void test_timeout_not_number()
{
    std::string input = R"(
    {
        "id": 1,
        "timeout": aaa
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, timeout must be a valid number", e.what());
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
        "timeout": -1
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, timeout must be a valid positive number", e.what());
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
        "timeout": 99999999999999999999999999999999999999999999999
    }
    )";

    try
    {
        auto _ = extractTimeOut(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, timeout must be a valid number", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}



void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_extract_string);
    RUN_TEST(test_extract_int);
    RUN_TEST(test_extract_json);
    RUN_TEST(test_extract_nested);
    RUN_TEST(test_extract_key_not_found);
    RUN_TEST(test_extract_missing_semicolon);
    RUN_TEST(test_extract_missing_closing_quotes);
    RUN_TEST(test_extract_missing_closing_square_bracket);

    RUN_TEST(test_valid_timeout);
    RUN_TEST(test_timeout_not_number);
    RUN_TEST(test_timeout_negative_number);
    RUN_TEST(test_timeout_overflow);

    UNITY_END();
}

void loop() {}