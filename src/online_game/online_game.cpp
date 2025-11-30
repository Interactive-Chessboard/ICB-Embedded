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


std::string setBoard(std::string request, std::atomic<bool>& end_task_flag)
{
    return "ok";
}


std::string makeMove(std::string request, std::atomic<bool>& end_task_flag)
{
    return "ok";
}


std::string animation(std::string request, std::atomic<bool>& end_task_flag)
{
    return "ok";
}





void runTask(std::string request_type, std::string request, std::string request_id,
             std::atomic<bool>& end_task_flag, std::atomic<bool>& task_running)
{
    task_running = true;
    std::string status;

    if (request_type == "set_board")
        status = setBoard(request, std::ref(end_task_flag));
    else if (request_type == "make_move")
        status = makeMove(request, std::ref(end_task_flag));
    else if (request_type == "animation")
        status = animation(request, std::ref(end_task_flag));
    else
        status = "Error, unknown request";

    if (!end_task_flag)
        Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, status));

    task_running = false;
}


void onlineGame()
{
    std::atomic<bool> end_task_flag{false};
    std::atomic<bool> task_running_flag{false};

    std::thread worker;

    while (true)
    {
        std::string request = Bluetooth::getBluetoothMessage();
        std::string request_id = extract_value(request, "id");
        std::string request_type = extract_value(request, "type");

        // Handle close and end task request regardless of if task is running
        if (request_type == "close" || request_type == "end_task")
        {
            end_task_flag = true;
            if (worker.joinable())
                worker.join();

            Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
            if (request_type == "close")
                return;

            end_task_flag = false;
            continue;
        }

        // Reject a request if a task is running
        if (task_running_flag)
        {
            Bluetooth::sendBluetoothMessage(
                makeReturnMsg(request_id, "Error, other task in progress")
            );
            continue;
        }

        // Start a new task
        if (request_type == "set_board" || request_type == "make_move" ||
            request_type == "animation")
        {
            worker = std::thread(runTask, request_type, request, request_id, 
                                 std::ref(end_task_flag), std::ref(task_running_flag));
        }
        else if (request_type == "available")
        {
            Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
        }
        else
        {
            Bluetooth::sendBluetoothMessage(
                makeReturnMsg(request_id, "Error, unknown request")
            );
        }
    }
}