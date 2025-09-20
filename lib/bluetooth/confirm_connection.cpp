// confirm_connection.cpp
#include "bluetooth.hpp"

// Sending format (ConfirmConnection: {})
// Receiving format (Received: {ACK})
bool Bluetooth::confirmConnection()
{
    Bluetooth::sendBluetoothMessage("ConfirmConnection: {}");
    std::string msg = Bluetooth::getBluetoothMessage();
    if (msg == "Received: {ACK}")
    {
        return true;
    }
    return false;
}