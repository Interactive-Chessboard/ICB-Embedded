// online_game.cpp
#include "online_game.hpp"


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& active)
{
    ExtractSetBoard extract_set_board;
    try
    {
        setClockSettings(clock_settings, request);
        extract_set_board = extractSetBoard(request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }
    SetBoard set_board(extract_set_board);
    return set_board.startOnline(active);
}


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& active)
{
    ExtractMakeMove extract_make_move;
    try
    {
        extract_make_move = extractMakeMove(request);
        setClockSettings(clock_settings, request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    MakeMove make_move(extract_make_move);
    Move move_made;
    try
    {
        make_move.startOnline(active);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    return "ok, \"move_from\": " + std::to_string(move_made.from_square) +
           ", \"move_to\": " + std::to_string(move_made.to_square) + "\"";
}


std::string animation(const std::string& request, std::atomic<bool>& active)
{
    std::string animation = extractValue(request, "animation");
    std::vector<Animation> animations;
    try
    {
        animations = extractAnimations(request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }
    return playAnimations(active, animations);
}


std::string makeReturnMsg(const std::string& request_id, const std::string& status)
{
    return "{\"id\": " + request_id + ", \"status\": \"" + status + "\"}";
}


void runTask(ClockSetting &clock_settings, std::string request_type, std::string request,
             std::string request_id, std::atomic<bool>& active, std::atomic<bool>& task_running)
{
    task_running = true;
    std::string status;

    if (request_type == "set_board")
        status = setBoard(clock_settings, request, active);

    else if (request_type == "make_move")
        status = makeMove(clock_settings, request, active);

    else if (request_type == "animation")
        status = animation(request, active);

    else
        status = "error, unknown request";

    Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, status));
    task_running = false;
}


void onlineGame(ClockSetting &clock_settings)
{
    std::atomic<bool> active{true};
    std::atomic<bool> task_running_flag{false};
    std::thread worker;

    while (true)
    {
        std::string request = Hardware::get().getBluetoothMessage();
        std::string request_id = extractValue(request, "id");
        std::string request_type = extractValue(request, "type");

        if (request_type == "ping")
        {
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "pong"));
            continue;
        }
        if (request_type == "info")
        {
            std::string mac_address = Hardware::get().getMacAddress();
            std::string version = Hardware::get().getFirmwareVersion();
            std::string msg = "{\"id\": " + request_id + ", \"status\": \"ok\", \"version\": " +
                               version + ", \"mac_address\": " + mac_address + "\"}";
            Hardware::get().sendBluetoothMessage(msg);
            continue;
        }
        // Control requests
        if (request_type == "close" || request_type == "end_task")
        {
            active.store(false);
            if (worker.joinable())
                worker.join();

            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
            if (request_type == "close")
                return;

            active.store(true);
            continue;
        }

        // Reject a request if a task is running
        if (task_running_flag)
        {
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "error, another task is in progress"));
            continue;
        }

        // Availabe request + 3 heavier request
        if (request_type == "set_board" || request_type == "make_move" ||
            request_type == "animation")
            worker = std::thread(runTask, std::ref(clock_settings), request_type, request, request_id,
                                 std::ref(active), std::ref(task_running_flag));

        else if (request_type == "available")
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "ok"));

        else
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "error, unknown request"));
    }
}
