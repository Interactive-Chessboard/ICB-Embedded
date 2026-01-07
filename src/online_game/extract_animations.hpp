// extract_animations.hpp
#pragma once
#include <vector>
#include <string>
#include <array>
#include <cctype>
#include <stdexcept>
#include "board.hpp"
#include "extract_value.hpp"

std::vector<Animation> parseAnimations(const std::string&);
