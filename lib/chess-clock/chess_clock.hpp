// chess_clock.hpp
#pragma once
#include <atomic>
#include <thread>
#include "setting_struct.hpp"
#include "clock_struct.hpp"

namespace ChessClock
{
void chess_clock(Settings, ClockSetting&);
};
