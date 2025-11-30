#include "online_game.hpp"


std::string extract_value(std::string str, std::string key)
{
    // ---- Find the key ----
    std::string quotedKey;
    quotedKey = "\"";
    quotedKey += key;
    quotedKey += "\"";

    size_t pos = str.find(quotedKey);
    if (pos == std::string::npos)
        return {};

    pos += quotedKey.size();

    // ---- Skip whitespace ----
    while (pos < str.size() && isspace(str[pos]))
        pos++;

    // ---- Expect ':' ----
    if (pos >= str.size() || str[pos] != ':')
        return {};
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
        if (end == std::string::npos)
            return {};
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

        if (i >= str.size())
            return {};

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


std::string makeReturnMsg(std::string request_id, std::string status)
{
    return "{\"id\": " + request_id + ", \"type\": \"" + status + "\"}";
}


std::string setBoard(std::string request)
{
    return "ok";
}


std::string makeMove(std::string request)
{
    return "ok";
}


std::string animation(std::string request)
{
    return "ok";
}


void onlineGame()
{
    while(true)
    {
        std::string request = Bluetooth::getBluetoothMessage();
        std::string request_id = extract_value(request, "id");

        std::string request_type = extract_value(request, "type");
        std::string status;
        if (request_type == "available")
        {
            status = "ok"; 
        }
        else if (request_type == "close")
        {
            Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
            return;
        }
        else if (request_type == "set_board")
        {
            status = setBoard(request);
        }
        else if (request_type == "make_move")
        {
            status = makeMove(request);
        }
        else if (request_type == "animation")
        {
            status = animation(request);
        }
        else
        {
            status = "Error, unkown request";
        }

        Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, status));
    }
}