// get_online_move.cpp
#include "bluetooth.hpp"

int string_to_int(std::string str, bool &err)
{
    int value;
    try
    {
        value = std::stoi(str);
    } 
    catch (const std::invalid_argument&) 
    {
        err = true;
    } 
    catch (const std::out_of_range&) 
    {
        err = true;
    }
    return value;
}



// Sending format (SendMove: {})
// Receiving format (FromSquare: {int}, ToSquare: {int}, WhiteTime: {int}, BlackTime: {int}, Winner: {String})
Move Bluetooth::getOnlineMove(ClockSetting &clock_settings, Winner &winner, const std::vector<Move> &legal_moves)
{
    for (int i = 0; i < 5; i++)
    {
        Bluetooth::sendBluetoothMessage("SendMove: {}");
        std::string received_msg = Bluetooth::getBluetoothMessage();
        bool error = false;

        // winner
        std::string winner_str = Bluetooth::extractValue(received_msg, "Winner");
        if (winner_str == "White")
        {
            winner =  Winner::White;
        }
        else if (winner_str == "Black")
        {
            winner =  Winner::Black;
        }
        else if (winner_str == "Draw")
        {
            winner = Winner::Draw;
        }
        else if (winner_str == "Nil")
        {
            winner = Winner::Nil;
        }
        else
        {
            continue;
        }

        // clock
        std::string time_white_str = Bluetooth::extractValue(received_msg, "WhiteTime");
        int time_white = string_to_int(time_white_str, error);
        if (error)
        {
            continue;
        }
        clock_settings.time_white = time_white;

        std::string time_black_str = Bluetooth::extractValue(received_msg, "WhiteTime");
        int time_black = string_to_int(time_black_str, error);
        if (error)
        {
            continue;
        }
        clock_settings.time_white = time_white;

        // move
        std::string from_square_str = Bluetooth::extractValue(received_msg, "FromSquare");
        int from_square = string_to_int(from_square_str, error);
        if (error)
        {
            continue;
        }
        std::string to_square_str = Bluetooth::extractValue(received_msg, "FromSquare");
        int to_square = string_to_int(to_square_str, error);
        if (error)
        {
            continue;
        }
        for(int i = 0; i < legal_moves.size(); i++)
        {
            if (legal_moves[i].from_square == from_square && legal_moves[i].to_square == to_square)
            {
                return legal_moves[i];
            }
        }

    }
    return Move{};
}