// extract_value.hpp
#pragma once
#include <string>
#include <stdexcept>
#include "game_clock/game_clock.hpp"


/**
 * @brief Extracts the raw value associated with a given key from a JSON-like string.
 *
 * This function searches for a quoted key in the input string and returns the
 * corresponding value as a string. The value may be:
 *   - a quoted string (quotes are removed),
 *   - a number (returned as-is),
 *   - an array or object (returned including surrounding brackets).
 *
 * Whitespace around keys, colons, and values is ignored. This function is
 * intentionally lightweight and does NOT implement full JSON parsing.
 *
 * Nested objects or arrays are supported and may be re-parsed by calling
 * extractValue again on the returned substring.
 *
 * @param str The input string containing a JSON-like object.
 * @param key The key whose value should be extracted.
 *
 * @return The extracted value as a string.
 *         - Strings are returned without surrounding quotes.
 *         - Numbers are returned as-is.
 *         - Objects and arrays are returned including enclosing braces/brackets.
 *
 * @throws std::runtime_error If any of the following conditions occur:
 *   - The key is not found ("Error, key not found").
 *   - The ':' character is missing after the key
 *     ("Error, expecting semicolon after key").
 *   - A quoted string value is missing a closing quote
 *     ("Error, expecting closing quotes").
 *   - An object or array value is missing its closing brace or bracket
 *     ("Error, expecting square bracket").
 *
 * @note This function does not validate full JSON syntax and assumes the input
 *       roughly follows a JSON-like structure. It does not handle escaped quotes,
 *       commas inside strings, or malformed nesting beyond basic bracket matching.
 */
std::string extractValue(const std::string&, const std::string&);


/**
 * @brief Extracts and validates the "timeout_s" value from a JSON-like string.
 *
 * This function retrieves the value associated with the `"timeout_s"` key using
 * extractValue(), converts it to an integer, multiplies it by 1000 and validates
 * that it is a strictly positive number.
 *
 * The input is expected to follow a JSON-like structure. Whitespace is ignored,
 * but the value must be a valid base-10 integer representation.
 *
 * @param str A string containing a JSON-like object with a `"timeout_s"` key.
 *
 * @return The timeout value as a positive integer in milliseconds.
 *
 * @throws std::runtime_error If any of the following conditions occur:
 *   - The `"timeout_s"` value cannot be parsed as an integer
 *     ("Error, timeout must be a valid number").
 *   - The `"timeout_s"` value is zero or negative
 *     ("Error, timeout must be a valid positive number").
 *
 * @note This function relies on extractValue() and inherits its limitations.
 *       It does not support floating-point values or full JSON validation.
 */
int extractTimeOut(const std::string &str);


/**
 * @brief Parse and apply clock configuration settings from a request string.
 *
 * This function extracts the "clock" object from the given request string and
 * updates the provided ClockSetting instance in a thread-safe manner.
 *
 * Expected clock fields:
 *  - active ("t" or "f")
 *      * "t" → clock is active and all clock values are updated
 *      * "f" → clock is inactive; function sets active to false and returns
 *              immediately without modifying any other clock values
 *  - white_ms (integer, >= 0): remaining time for White in milliseconds
 *  - black_ms (integer, >= 0): remaining time for Black in milliseconds
 *  - extra_time_ms (integer, >= 0): increment added after each move
 *  - run_down ("w" or "b"): indicates which player's clock is currently running
 *
 * Thread safety:
 *  - All modifications to ClockSetting are protected by an internal mutex.
 *  - Atomic members are updated while holding the mutex to ensure consistency.
 *
 * Error handling:
 *  - Throws std::runtime_error with the following messages:
 *      * "Error, expecting true (t) or false (f)"
 *          - if the "active" field is not "t" or "f"
 *      * "Error, time values must be valid"
 *          - if any time field is missing, non-numeric, or causes conversion failure
 *      * "Error, time values must be positive"
 *          - if any time value is negative
 *      * "Error, invalid run down clock color"
 *          - if "run_down" is not "w" or "b"
 *
 * @param clock_settings Reference to the ClockSetting object to update.
 * @param request        String containing the serialized request with clock data.
 *
 * @throws std::runtime_error on invalid or malformed clock settings.
 */
void setClockSettings(ClockSetting&, const std::string&);


/**
 * @brief Parse a string representation of an RGB LED color.
 *
 * This function parses a color string formatted as:
 *     "[R, G, B]"
 * where R, G, and B are integer values.
 *
 * The function:
 * - Extracts the first three integer values found in the brackets.
 * - Ignores any additional values beyond the first three.
 * - Validates that exactly three integers are successfully parsed.
 * - Ensures each color component is within the range [0, 255].
 *
 * @param color_str A string containing the LED color in bracketed RGB format.
 *
 * @return LedColor An instance of LedColor initialized with the parsed RGB values.
 *
 * @throws std::runtime_error If:
 *         - The input string does not match the expected "[R, G, B]" format.
 *         - Fewer than three integer values are present.
 *         - Any color value is negative.
 *         - Any color value is greater than 255.
 *
 * @note Strings containing more than three values (e.g. "[55, 17, 35, 83]")
 *       are considered valid; only the first three values are used.
 */
LedColor getLedColor(const std::string &);
