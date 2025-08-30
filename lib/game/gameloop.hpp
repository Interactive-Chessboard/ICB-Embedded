// gameloop.hpp
#pragma once
#include <atomic>
#include "settings.hpp"


char gameLoop(Settings game_settings, std::atomic<bool>& active);