// game.hpp
#pragma once
#include <thread>
#include <string>
#include "bluetooth.hpp"
#include "board.hpp"
#include "chess.hpp"
#include "stockfish.hpp"
#include "structs.hpp"

namespace Game
{
void game(Settings);
};
