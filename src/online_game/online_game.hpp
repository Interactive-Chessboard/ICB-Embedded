// online_game.hpp
#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <string>
#include "bluetooth.hpp"
#include "extract_animations.hpp"
#include "extract_value.hpp"
#include "extract_set_board.hpp"
#include "game_clock/game_clock.hpp"
#include "extract_make_move.hpp"
#include "make_move/make_move.hpp"
#include "set_board/set_board.hpp"


void onlineGame(ClockSetting&);
