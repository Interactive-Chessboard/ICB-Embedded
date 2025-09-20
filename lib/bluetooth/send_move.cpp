// send_move.cpp
#include "bluetooth.hpp"

// Sending format (FromSquare: {int}, ToSquare: {int})
// Receiving format (Winner: {string})
Winner Bluetooth::sendMove(const Move &move)
{
    for (int i = 0; i < 5; i++)
    {
        std::string from_square = std::to_string(move.from_square);
        std::string to_square = std::to_string(move.to_square);
        Bluetooth::sendBluetoothMessage("FromSquare: {" + from_square + "}, ToSquare: {" + to_square + "}");
        std::string received_msg = Bluetooth::getBluetoothMessage();
        std::string winner = Bluetooth::extractValue(received_msg, "Winner");
        if (winner == "White")
        {
            return Winner::White;
        }
        else if (winner == "Black")
        {
            return Winner::Black;
        }
        else if (winner == "Draw")
        {
            return Winner::Draw;
        }
        else if (winner == "Nil")
        {
            return Winner::Nil;
        }
    }
    return Winner::Nil;
}