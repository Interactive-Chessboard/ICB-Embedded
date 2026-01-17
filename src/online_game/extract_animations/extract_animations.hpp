// extract_animations.hpp
#pragma once
#include <vector>
#include <string>
#include <array>
#include <cctype>
#include <stdexcept>
#include "hardware.hpp"
#include "../extract_value/extract_value.hpp"
#include "animations/animations.hpp"


/**
 * @brief Extracts LED board animations from a JSON-like text description.
 *
 * This function parses a string containing an array of animation objects.
 * Each animation object must contain:
 *   - a `"board"` key associated with an array of exactly 64 RGB color triplets,
 *   - a `"time_ms"` key associated with a positive integer display duration.
 *
 * The expected board format is:
 *   [ [R,G,B], [R,G,B], ... ]  (exactly 64 elements)
 * where R, G, and B are integers in the range [0, 255].
 *
 * Whitespace is ignored. The parser is tolerant to spacing but strict about
 * structure and value validity. This is NOT a full JSON parser and only
 * supports the specific expected format.
 *
 * @param text A string containing the animation definitions.
 *
 * @return A vector of Animation objects parsed from the input.
 *
 * @throws std::runtime_error If any of the following conditions occur:
 *   - The `"board"` key is missing.
 *   - The `"time_ms"` key is missing.
 *   - The ':' character is missing after `"time_ms"`.
 *   - The board format is invalid.
 *   - The number of board squares is not exactly 64.
 *   - A color component or time value is negative.
 *   - A color component exceeds 255.
 *   - A color component or time value is not a valid number.
 *
 * @note This function performs manual string parsing and does not validate
 *       full JSON syntax. Inputs that deviate from the expected structure
 *       may result in exceptions.
 */
std::vector<Animation> extractAnimations(const std::string&);
