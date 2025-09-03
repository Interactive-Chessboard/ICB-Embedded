// game.hpp
#pragma once
#include <thread>
#include "setting_struct.hpp"
#include "clock_struct.hpp"
#include "chess_game_struct.hpp"
#include "board.hpp"
#include "game_end.hpp"
#include "game_settings.hpp"
#include "legal_moves.hpp"
#include "opponent.hpp"
#include "setting_struct.hpp"
#include "chess_clock.hpp"


namespace Game
{
void game(Settings);
};
