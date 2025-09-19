// chess_clock.hpp
#pragma once
#include <atomic>
#include <thread>
#include "structs.hpp"

namespace ChessClock
{
Winner chess_clock_offline(ClockSetting&);
Winner chess_clock_online(ClockSetting&);
};
