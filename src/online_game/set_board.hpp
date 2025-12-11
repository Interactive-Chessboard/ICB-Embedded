// set_board.hpp
#pragma once
#include <chrono>
#include <stdexcept>
#include "board.hpp"
#include "extract_value.hpp"
#include "game_clock/game_clock.hpp"


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag);