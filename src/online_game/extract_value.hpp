// extract_value.hpp
#pragma once
#include <string>
#include <stdexcept>
#include "game_clock/game_clock.hpp"


std::string extract_value(const std::string& str, const std::string& key);
int extractTimeOut(const std::string &str);
void setClockSettings(ClockSetting &clock_settings, const std::string& request);
LedColor getColor(const std::string &color_str);