// clock.hpp
#pragma once
#include <atomic>
#include "structs.hpp"

namespace Clock
{
void clock(GameSettings game_settings, std::atomic<bool>& active);
};

// To set the bool to false do this
// active.store(false);