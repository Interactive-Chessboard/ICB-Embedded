// test_extract_set_board.cpp
#include <unity.h>
#include "../test_helper.hpp"
#include "online_game/extract_set_board/extract_set_board.hpp"



void test_functional_set_board()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 18446462598732906495,
        "color": [0, 0, 255],
        "clock": {
            "active": "f",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";
    ExtractSetBoard extract_set_board;
    try
    {
        extract_set_board = extractSetBoard(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_set_board.color);
    uint64_t expected = std::stoull("18446462598732906495");
    TEST_ASSERT_UINT64_T(expected, extract_set_board.board);
    TEST_ASSERT_EQUAL(60000, extract_set_board.timeout);
}


void test_functional_set_board2()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 189995,
        "color": [0, 0, 255],
        "clock": {
            "active": "f",
            "white_ms": 50000,
            "black_ms": 60000,
            "extra_time_ms": 5000,
            "run_down": "w",
        },
        "timeout_s": 60
    }
    )";
    ExtractSetBoard extract_set_board;
    try
    {
        extract_set_board = extractSetBoard(input);
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("No exceptions were expected");
    }
    TEST_ASSERT_EQUAL_LED_COLOR(LedColor(0, 0, 255), extract_set_board.color);
    uint64_t expected = std::stoull("189995");
    TEST_ASSERT_UINT64_T(expected, extract_set_board.board);
    TEST_ASSERT_EQUAL(60000, extract_set_board.timeout);
}


void test_set_board_overflow()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": 1844646259873290649599999999999,
        "color": [0, 0, 255],
        "clock": {
            "active": "f",
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
        auto _ = extractSetBoard(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, number must be a valid uint64", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void test_set_board_negative()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": -1454662581732942365,
        "color": [0, 0, 255],
        "clock": {
            "active": "f",
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
        auto _ = extractSetBoard(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, number must be a valid uint64", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}



void test_set_board_non_number_board()
{
    std::string input = R"(
    {
        "id": 1,
        "type": "set_board",
        "board": abcdef,
        "color": [0, 0, 255],
        "clock": {
            "active": "f",
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
        auto _ = extractSetBoard(input);
        TEST_FAIL_MESSAGE("Expected exception");
    }
    catch (const std::runtime_error& e)
    {
        TEST_ASSERT_EQUAL_STRING("Error, number must be a valid uint64", e.what());
    }
    catch (...)
    {
        TEST_FAIL_MESSAGE("Expected std::runtime_error but caught a different exception type.");
    }
}


void runTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_functional_set_board);
    RUN_TEST(test_functional_set_board2);
    RUN_TEST(test_set_board_overflow);
    RUN_TEST(test_set_board_negative);
    RUN_TEST(test_set_board_non_number_board);

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
