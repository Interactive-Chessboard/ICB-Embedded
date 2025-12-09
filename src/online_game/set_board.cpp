// set_board.cpp
#include "set_board.hpp"


void setClockSettings(ClockSetting &clock_settings, const std::string& request)
{
    std::string clock_settings_str = extract_value(request, "clock");

    std::string active_str = extract_value(clock_settings_str, "active");
    bool active;
    if (active_str == "true")
        active = true;
    else if (active_str == "false")
        active = false;
    else
        throw std::runtime_error("Error, expecting square bracket");

    int white, black, extra_time;
    try
    {
        white = stoi(extract_value(clock_settings_str, "white"));
        black = stoi(extract_value(clock_settings_str, "black"));
        extra_time = stoi(extract_value(clock_settings_str, "extra_time"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, time values are not ints");
    }
    if (white < 0 || black < 0 || extra_time < 0) throw std::runtime_error("Error, time values can't be negative");
    


    std::string run_down = extract_value(clock_settings_str, "run_down");
    ClockColor clock_color;
    if (run_down == "w")
        clock_color = ClockColor::White;
    else if (run_down == "b")
        clock_color = ClockColor::Black;
    else
        throw std::runtime_error("Error, expecting square bracket");

    {
    std::lock_guard<std::mutex> lock(clock_settings.mtx);

    clock_settings.active.store(active);
    clock_settings.time_white.store(white * 100);
    clock_settings.time_black.store(black * 100);
    clock_settings.extra_time.store(extra_time * 100);
    clock_settings.player_turn.store(clock_color);
    }
}


LedColor getPastMoveColor(const std::string &request)
{
    std::string color_str = extract_value(extract_value(request, "old_move"), "color");
    int r, g, b;
    std::sscanf(color_str.c_str(), "[%d, %d, %d]", &r, &g, &b);
    if (r > 255 || g > 255 || b > 255) throw std::runtime_error("Error, colors must be lower than 256");
    if (r < 0 || g < 0 || b < 0) throw std::runtime_error("Error, colors must be positive");
    return LedColor(r, g, b);
}


uint64_t to_uint64(const std::string &request)
{
    try
    {
        return std::stoull(extract_value(request, "board"));
    }
    catch (...)
    {
        throw std::runtime_error("Error, number must be a valid uint64");
    }
}


std::array<LedColor, 64> lightUpDifference(uint64_t current, uint64_t desired, LedColor color)
{
    std::array<LedColor, 64> result{};
    uint64_t diff = current ^ desired;

    for (int i = 0; i < 64; ++i)
    {
        if (diff & (1ULL << i)) {
            result[i] = color;
        } else {
            result[i] = LedColor();
        }
    }

    return result;
}


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    LedColor past_move_color;
    uint64_t board;
    int timeout;
    try
    {
        setClockSettings(std::ref(clock_settings), request);
        past_move_color = getPastMoveColor(request);
        board = to_uint64(request);
        timeout = extractTimeOut(request) * 100;
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    while (!end_task_flag.load() && timeout < 0)
    {
        uint64_t current_board = Board::getBoardArr();
        if (current_board == board)
        {
            return "ok";
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, board, past_move_color);
        Board::setLed(lights); 
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timeout--;
    }
    return "Error, timeout reached";
}