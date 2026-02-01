// extract_value.cpp
#include "extract_value.hpp"


std::string extractValue(const std::string &str, const std::string &key)
{
    // ---- Find the key ----
    std::string quotedKey;
    quotedKey = "\"";
    quotedKey += key;
    quotedKey += "\"";

    size_t pos = str.find(quotedKey);
    if (pos == std::string::npos) throw std::runtime_error("Error, key not found");

    pos += quotedKey.size();

    // ---- Skip whitespace ----
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    // ---- Expect ':' ----
    if (pos >= str.size() || str[pos] != ':') throw std::runtime_error("Error, expecting semicolon after key");
    pos++;

    // ---- Skip whitespace ----
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    // ---- Determine value type ----
    char c = str[pos];

    if (c == '"')
    {
        size_t start = pos + 1;
        size_t end = str.find('"', start);
        if (end == std::string::npos) throw std::runtime_error("Error, expecting closing quotes");
        return str.substr(start, end - start);
    }

    if (c == '{' || c == '[')
    {
        char open = c;
        char close = (c == '{') ? '}' : ']';
        size_t depth = 0;
        size_t i = pos;

        for (; i < str.size(); i++)
        {
            if (str[i] == open)
                depth++;
            else if (str[i] == close)
            {
                depth--;
                if (depth == 0)
                    break;
            }
        }

        if (i >= str.size()) throw std::runtime_error("Error, expecting square bracket");

        return str.substr(pos, i - pos + 1);
    }

    size_t start = pos;
    while (pos < str.size() &&
           str[pos] != ',' &&
           str[pos] != '}' &&
           str[pos] != ']' &&
           !isspace(str[pos]))
        pos++;

    return str.substr(start, pos - start);
}


int extractTimeOut(const std::string &str)
{
    int timeout;
    try
    {
        int timeout_s = stoi(extractValue(str, "timeout_s"));
        timeout = timeout_s * 1000;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error, timeout must be a valid number");
    }
    if (timeout <= 0) throw std::runtime_error("Error, timeout must be a valid positive number");
    return timeout;
}


void setClockSettings(ClockSetting &clock_settings, const std::string &request)
{
    std::string clock_settings_str = extractValue(request, "clock");

    std::lock_guard<std::mutex> lock(clock_settings.mtx);
    {
    // Extract active state
    std::string active_str = extractValue(clock_settings_str, "active");
    if (active_str == "f")
    {
        clock_settings.active.store(false);
        return;
    }
    if (active_str == "t")
        clock_settings.active.store(true);
    else
        throw std::runtime_error("Error, expecting true (t) or false (f)");

    // Extract time
    int white, black, extra_time;
    try
    {
        white = stoi(extractValue(clock_settings_str, "white_ms"));
        black = stoi(extractValue(clock_settings_str, "black_ms"));
        extra_time = stoi(extractValue(clock_settings_str, "extra_time_ms"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, time values must be valid");
    }
    if (white < 0 || black < 0 || extra_time < 0) throw std::runtime_error("Error, time values must be positive");
    clock_settings.time_white.store(white);
    clock_settings.time_black.store(black);
    clock_settings.extra_time.store(extra_time);

    // Extract clock player turn
    std::string run_down = extractValue(clock_settings_str, "run_down");
    Color clock_color;
    if (run_down == "w")
        clock_color = Color::White;
    else if (run_down == "b")
        clock_color = Color::Black;
    else
        throw std::runtime_error("Error, invalid run down clock color");
    clock_settings.player_turn.store(clock_color);
    }
}


LedColor getLedColor(const std::string &color_str)
{
    int r, g, b;
    int parsed = std::sscanf(color_str.c_str(), "[%d, %d, %d]", &r, &g, &b);

    if (parsed != 3)
        throw std::runtime_error("Error, invalid LED color format");
    if (r > 255 || g > 255 || b > 255)
        throw std::runtime_error("Error, colors must be lower than 256");
    if (r < 0 || g < 0 || b < 0)
        throw std::runtime_error("Error, colors must be positive");
    return LedColor(r, g, b);
}
