// extract_set_board.hpp
#pragma once
#include <stdexcept>
#include "../extract_value/extract_value.hpp"
#include "set_board/set_board.hpp"


/**
 * @brief Extracts and validates a "set_board" command from a JSON request string.
 *
 * This function parses a JSON-formatted request and constructs an
 * ExtractSetBoard structure containing:
 *  - the LED color,
 *  - the chess board state as a uint64_t,
 *  - the timeout value in milliseconds.
 *
 * The "board" field must represent a valid unsigned 64-bit integer.
 * If the board value is negative, non-numeric, or outside the uint64_t range,
 * a std::runtime_error is thrown.
 *
 * @param request A JSON string containing a "set_board" command.
 *
 * @return An ExtractSetBoard object populated with the extracted values.
 *
 * @throws std::runtime_error If the board value is not a valid uint64_t.
 */
ExtractSetBoard extractSetBoard(const std::string&);

