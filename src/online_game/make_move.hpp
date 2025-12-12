// make_move.hpp
#pragma once
#include <atomic>
#include <string>
#include <stdexcept>
#include <vector>
#include "game_clock/game_clock.hpp"
#include "extract_value.hpp"
#include "chess.hpp"
#include "set_board.hpp" //temp


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag);