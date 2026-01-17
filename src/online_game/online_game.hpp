// online_game.hpp
#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <string>
#include "hardware.hpp"
#include "extract_animations/extract_animations.hpp"
#include "extract_value/extract_value.hpp"
#include "extract_set_board/extract_set_board.hpp"
#include "game_clock/game_clock.hpp"
#include "extract_make_move/extract_make_move.hpp"
#include "make_move/make_move.hpp"
#include "set_board/set_board.hpp"
#include "animations/animations.hpp"


void onlineGame(ClockSetting&);
