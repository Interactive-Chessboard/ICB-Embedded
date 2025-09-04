// game.hpp
#pragma once
#include <thread>
#include "board.hpp"
#include "game_end.hpp"
#include "chess_clock.hpp"
#include "game_settings.hpp"
#include "legal_moves.hpp"
#include "opponent.hpp"
#include "structs.hpp"

namespace Game
{
void game(Settings);
};
