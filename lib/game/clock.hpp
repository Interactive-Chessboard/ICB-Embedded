// clock.hpp
#pragma once
#include <atomic>
#include <thread>
#include "settings.hpp"


void chess_clock(Settings game_settings, std::atomic<bool>& active);

// To set the bool to false do this
// active.store(false);