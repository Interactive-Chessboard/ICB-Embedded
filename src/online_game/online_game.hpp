// online_game.hpp
#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <string>
#include "structs.hpp"
#include "bluetooth.hpp"
#include "animations.hpp"
#include "extract_value.hpp"
#include "set_board.hpp"
#include "game_clock/game_clock.hpp"

void onlineGame(ClockSetting &clock_settings);
