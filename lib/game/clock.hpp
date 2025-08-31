// clock.hpp
#pragma once
#include <atomic>
#include <thread>
#include "setting_struct.hpp"
#include "clock_struct.hpp"


void chess_clock(Settings, ClockSetting&);

// To set the bool to false do this
// active.store(false);