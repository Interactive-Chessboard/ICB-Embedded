// local_game.hpp
#pragma once
#include <thread>
#include <string>
#include <vector>
#include "hardware.hpp"
#include "chess.hpp"
#include "game_settings/game_settings.hpp"
#include "game_clock/game_clock.hpp"
#include "set_board/set_board.hpp"
#include "make_move/make_move.hpp"


void localGame(Settings, ClockSetting&);
