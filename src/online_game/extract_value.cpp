// extract_value.cpp
#include "extract_value.hpp"


std::string extract_value(const std::string& str, const std::string& key)
{
    // ---- Find the key ----
    std::string quotedKey;
    quotedKey = "\"";
    quotedKey += key;
    quotedKey += "\"";

    size_t pos = str.find(quotedKey);
    if (pos == std::string::npos) throw std::runtime_error("Error, key not found");

    pos += quotedKey.size();

    // ---- Skip whitespace ----
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    // ---- Expect ':' ----
    if (pos >= str.size() || str[pos] != ':') throw std::runtime_error("Error, expecting semicolon after key");
    pos++;

    // ---- Skip whitespace ----
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    // ---- Determine value type ----
    char c = str[pos];

    if (c == '"')
    {
        size_t start = pos + 1;
        size_t end = str.find('"', start);
        if (end == std::string::npos) throw std::runtime_error("Error, expecting closing quotes");
        return str.substr(start, end - start);
    }

    if (c == '{' || c == '[')
    {
        char open = c;
        char close = (c == '{') ? '}' : ']';
        size_t depth = 0;
        size_t i = pos;

        for (; i < str.size(); i++)
        {
            if (str[i] == open)
                depth++;
            else if (str[i] == close)
            {
                depth--;
                if (depth == 0)
                    break;
            }
        }

        if (i >= str.size()) throw std::runtime_error("Error, expecting square bracket");

        return str.substr(pos, i - pos + 1);
    }

    size_t start = pos;
    while (pos < str.size() &&
           str[pos] != ',' &&
           str[pos] != '}' &&
           str[pos] != ']' &&
           !isspace(str[pos]))
        pos++;

    return str.substr(start, pos - start);
}


int extractTimeOut(const std::string &str)
{
    int timeout;
    try
    {
        timeout = stoi(extract_value(str, "timout"));
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error, timeout must be a number");
    }
    if (timeout <= 0) throw std::runtime_error("Error, timeout must be positive");
    return timeout;
}
    