// chess_clock.hpp
#pragma once
#include <atomic>
#include <thread>
#include <chrono>
#include "structs.hpp"

namespace ChessClock
{
Winner chess_clock(ClockSetting&);
};
