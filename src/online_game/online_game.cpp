// online_game.cpp
#include "online_game.hpp"


std::string makeReturnMsg(const std::string& request_id, const std::string& status)
{
    return "{\"id\": " + request_id + ", \"type\": \"" + status + "\"}";
}


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    return "ok";
}


void runTask(ClockSetting &clock_settings, std::string request_type, std::string request, 
             std::string request_id, std::atomic<bool>& end_task_flag, std::atomic<bool>& task_running)
{
    task_running = true;
    std::string status;

    if (request_type == "set_board")
        status = setBoard(std::ref(clock_settings), request, std::ref(end_task_flag));
    else if (request_type == "make_move")
        status = makeMove(std::ref(clock_settings), request, std::ref(end_task_flag));
    else if (request_type == "animation")
        status = animation(request, std::ref(end_task_flag));
    else
        status = "Error, unknown request";

    if (!end_task_flag)
        Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, status));

    task_running = false;
}


void onlineGame(ClockSetting &clock_settings)
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
            end_task_flag.store(true);
            if (worker.joinable())
                worker.join();

            Bluetooth::sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
            if (request_type == "close")
                return;

            end_task_flag.store(false);
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
            worker = std::thread(runTask, std::ref(clock_settings), request_type, request, request_id, 
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