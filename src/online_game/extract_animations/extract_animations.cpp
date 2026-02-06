// extract_animations.cpp
#include "extract_animations.hpp"


inline void skipSpaces(const std::string& s, int& i)
{
    while (i < s.size() && std::isspace(s[i])) i++;
}


int parseInt(const std::string& s, int& i)
{
    skipSpaces(s, i);
    if (s[i] == '-') throw std::runtime_error("error, colors or timeout must be positive");
    int val = 0;
    while (i < s.size()) {
        if (!isdigit(s[i])) {
            if (s[i] == ',' || s[i] == ']' || std::isspace(s[i]))
                break;
            throw std::runtime_error("error, colors or timeout must be a number");
        }
        val = val * 10 + (s[i] - '0');
        i++;
    }
    return val;
}


LedColor parseColorTriplet(const std::string& s, int& i)
{
    skipSpaces(s, i);
    if (s[i] != '[') return LedColor();
    i++;

    int r = parseInt(s, i);
    skipSpaces(s, i); if (s[i] == ',') i++;

    int g = parseInt(s, i);
    skipSpaces(s, i); if (s[i] == ',') i++;

    int b = parseInt(s, i);
    skipSpaces(s, i);

    if (r > 255 || g > 255 || b > 255) throw std::runtime_error("error, colors must be lower than 256");

    if (s[i] == ']') i++;

    return LedColor(r, g, b);
}


void parseBoard(const std::string& s, int& i, std::array<LedColor,64>& leds)
{
    skipSpaces(s, i);

    int pos = s.find("\"board\"", i);
    if (pos == std::string::npos) throw std::runtime_error("error, board key not found");
    i = pos + 7;

    pos = s.find('[', i);
    if (pos == std::string::npos) throw std::runtime_error("error, invalid format");
    i = pos + 1;

    int idx = 0;
    while (i < s.size())
    {
        skipSpaces(s, i);
        if (s[i] == '[')
        {
            leds[idx++] = parseColorTriplet(s, i);
            skipSpaces(s, i);
            if (s[i] == ',') i++;
        }
        else if (s[i] == ']')
        {
            i++;
            break;
        }
        else
            i++;
    }
    if (idx != 64) throw std::runtime_error("error, number of squares must be 64");
}


int parseTimeMs(const std::string& s, int& i)
{
    int pos = s.find("\"time_ms\"", i);
    if (pos == std::string::npos) throw std::runtime_error("error, time_ms key not found");
    i = pos + 9;

    pos = s.find(':', i);
    if (pos == std::string::npos) throw std::runtime_error("error, : not found after time_ms");
    i = pos + 1;

    return parseInt(s, i);
}


std::vector<Animation> extractAnimations(const std::string& text)
{
    std::vector<Animation> result;

    int i = 0;
    while (i < text.size())
    {
        int objStart = text.find('{', i);
        if (objStart == std::string::npos) break;
        i = objStart + 1;

        Animation anim;

        parseBoard(text, i, anim.leds);
        anim.display_time_ms = parseTimeMs(text, i);

        result.push_back(anim);

        int objEnd = text.find('}', i);
        if (objEnd == std::string::npos) break;
        i = objEnd + 1;
    }

    return result;
}
