// game.hpp
#pragma once
#include <thread>
#include <string>
#include "board.hpp"
#include "bluetooth.hpp"
#include "game_end.hpp"
#include "chess_clock.hpp"
#include "game_settings.hpp"
#include "legal_moves.hpp"
#include "bots.hpp"
#include "structs.hpp"

namespace Game
{
void game(Settings);
};
