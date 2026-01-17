// extract_set_board.hpp
#pragma once
#include <chrono>
#include <stdexcept>
#include "hardware.hpp"
#include "../extract_value/extract_value.hpp"


struct ExtractSetBoard
{
    LedColor color;
    uint64_t board;
    int timeout;
};

ExtractSetBoard extractSetBoard(const std::string& request);
