// game.hpp
#pragma once
#include "clock.hpp"
#include "gameloop.hpp"
#include "gameend.hpp"

namespace Game
{
void game(Settings);
};

// To set the bool to false do this
// active.store(false);