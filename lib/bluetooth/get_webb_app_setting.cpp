// get_web_app_settings.cpp
#include "bluetooth.hpp"

// Sending format (SendSettings: {})
// Receiving format (Color: {string}, GameTime: {int}, ExtraTime: {int})
Settings Bluetooth::getWebAppSettings()
{
    Settings game_settings;
    for (int i = 0; i < 5; i++)
    {
        Bluetooth::sendBluetoothMessage("SendSettings: {}");
        std::string received_msg = Bluetooth::getBluetoothMessage();
        bool error = false;

        // color
        std::string color_str = Bluetooth::extractValue(received_msg, "Color");
        if (color_str == "White")
        {
            game_settings.player_color = Color::White;
        }
        else if (color_str == "Black")
        {
            game_settings.player_color = Color::Black;
        }
        else
        {
            continue;
        }

        // game time
        std::string game_time_str = Bluetooth::extractValue(received_msg, "GameTime");
        int game_time = Bluetooth::string_to_int(game_time_str, error);
        if (error)
        {
            continue;
        }
        game_settings.game_time_min = game_time;

        // game extra time
        std::string game_extra_time_str = Bluetooth::extractValue(received_msg, "ExtraTime");
        int game_extra_time = Bluetooth::string_to_int(game_extra_time_str, error);
        if (error)
        {
            continue;
        }
        game_settings.extra_time_sec = game_extra_time;
    }
    return game_settings;
}