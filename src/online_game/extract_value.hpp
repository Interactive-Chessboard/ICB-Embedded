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
 * extract_value again on the returned substring.
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
std::string extract_value(const std::string&, const std::string&);


/**
 * @brief Extracts and validates the "timeout" value from a JSON-like string.
 *
 * This function retrieves the value associated with the `"timeout"` key using
 * extract_value(), converts it to an integer, and validates that it is a
 * strictly positive number.
 *
 * The input is expected to follow a JSON-like structure. Whitespace is ignored,
 * but the value must be a valid base-10 integer representation.
 *
 * @param str A string containing a JSON-like object with a `"timeout"` key.
 *
 * @return The timeout value as a positive integer.
 *
 * @throws std::runtime_error If any of the following conditions occur:
 *   - The `"timeout"` value cannot be parsed as an integer
 *     ("Error, timeout must be a valid number").
 *   - The `"timeout"` value is zero or negative
 *     ("Error, timeout must be a valid positive number").
 *
 * @note This function relies on extract_value() and inherits its limitations.
 *       It does not support floating-point values or full JSON validation.
 */
int extractTimeOut(const std::string &str);


void setClockSettings(ClockSetting &clock_settings, const std::string& request);
LedColor getColor(const std::string &color_str);
