// online_game.cpp
#include "online_game.hpp"


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    ExtractSetBoard extract_set_board;
    try
    {
        setClockSettings(std::ref(clock_settings), request);
        extract_set_board = extractSetBoard(request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }
    SetBoard set_board(extract_set_board.color, extract_set_board.board);
    return set_board.startOnline(std::ref(end_task_flag), extract_set_board.timeout);
}


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    ExtractMakeMove extract_move;
    try
    {
        extract_move = extractMakeMove(request);
        setClockSettings(std::ref(clock_settings), request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    MakeMove make_move(extract_move.game, extract_move.past_move_from, extract_move.past_move_to,
                       extract_move.past_move_color, extract_move.lifted_square_color,
                       extract_move.legal_moves_color, extract_move.illegal_moves_color);
    Move move_made;
    try
    {
        make_move.startOnline(std::ref(end_task_flag), extract_move.timeout);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    return "ok, \"move_from\": " + std::to_string(move_made.from_square) +
           ", \"move_to\": " + std::to_string(move_made.to_square) + "\"";
}


std::string animation(const std::string& request, std::atomic<bool>& end_task_flag)
{
    std::string animation = extract_value(request, "animation");
    std::vector<Animation> animations;
    try
    {
        animations = parseAnimations(request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }
    return playAnimations(std::ref(end_task_flag), animations);
}


std::string makeReturnMsg(const std::string& request_id, const std::string& status)
{
    return "{\"id\": " + request_id + ", \"status\": \"" + status + "\"}";
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

    Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, status));
    task_running = false;
}


void onlineGame(ClockSetting &clock_settings)
{
    std::atomic<bool> end_task_flag{false};
    std::atomic<bool> task_running_flag{false};
    std::thread worker;

    while (true)
    {
        std::string request = Hardware::get().getBluetoothMessage();
        std::string request_id = extract_value(request, "id");
        std::string request_type = extract_value(request, "type");

        // Handle close and end task request regardless of if task is running
        if (request_type == "close" || request_type == "end_task")
        {
            end_task_flag.store(true);
            if (worker.joinable())
                worker.join();

            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "ok"));
            if (request_type == "close")
                return;

            end_task_flag.store(false);
            continue;
        }

        // Reject a request if a task is running
        if (task_running_flag)
        {
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "Error, other task in progress"));
            continue;
        }

        // Start a new task
        if (request_type == "set_board" || request_type == "make_move" ||
            request_type == "animation")
            worker = std::thread(runTask, std::ref(clock_settings), request_type, request, request_id,
                                 std::ref(end_task_flag), std::ref(task_running_flag));

        else if (request_type == "available")
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "ok"));

        else
            Hardware::get().sendBluetoothMessage(makeReturnMsg(request_id, "Error, unknown request"));
    }
}
