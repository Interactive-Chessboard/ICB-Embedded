// get_online_move.cpp
#include "bluetooth.hpp"


// Sending format (SendMove: {})
// Receiving format (FromSquare: {int}, ToSquare: {int}, WhiteTime: {int}, BlackTime: {int}, Winner: {String})
std::pair<Move, Winner> Bluetooth::getOnlineMove(ClockSetting &clock_settings, const std::vector<Move> &legal_moves)
{
    Winner winner;
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
        int time_white = Bluetooth::string_to_int(time_white_str, error);
        if (error)
        {
            continue;
        }
        clock_settings.time_white.store(time_white);

        std::string time_black_str = Bluetooth::extractValue(received_msg, "BlackTime");
        int time_black = Bluetooth::string_to_int(time_black_str, error);
        if (error)
        {
            continue;
        }
        clock_settings.time_black.store(time_black);

        // move
        std::string from_square_str = Bluetooth::extractValue(received_msg, "FromSquare");
        int from_square = Bluetooth::string_to_int(from_square_str, error);
        if (error)
        {
            continue;
        }
        std::string to_square_str = Bluetooth::extractValue(received_msg, "ToSquare");
        int to_square = Bluetooth::string_to_int(to_square_str, error);
        if (error)
        {
            continue;
        }
        for(int i = 0; i < legal_moves.size(); i++)
        {
            if (legal_moves[i].from_square == from_square && legal_moves[i].to_square == to_square)
            {
                return {legal_moves[i], winner};
            }
        }

    }
    return {Move{}, winner};
}