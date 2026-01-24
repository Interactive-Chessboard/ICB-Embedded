// screen_selection
#include "screen_selection.hpp"


int selectOption(std::vector<std::string> options, const std::atomic<bool>& active)
{
    int selected = 0;
    Hardware::get().reserveScreen(true);
    while(active.load())
    {
        Hardware::get().setScreen(options, selected);
        if (Hardware::get().detectStartClick())
            return selected;
        if (Hardware::get().detectSelectClick())
            selected = (selected + 1) % options.size();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    Hardware::get().reserveScreen(false);
    return selected;
}


Settings getGameSettings()
{
    std::atomic<bool> active{true};
    Settings settings;

    // Gamemode
    std::vector<std::string> game_mode_options = {"Online", "Local 1v1", "Local bot"};
    int game_mode_option = selectOption(game_mode_options, active);
    switch (game_mode_option)
    {
    case 0:
        settings.game_mode = GameMode::Online;
        return settings;
    case 1:
        settings.game_mode = GameMode::MultiplayerOffline;
        break;
    case 2:
        settings.game_mode = GameMode::BotsOffline;
        break;
    default:
        return settings;
    }

    // Player color
    std::vector<std::string> color_options = {"White", "Black", "Random"};
    int color_option = selectOption(color_options, active);
    switch (color_option)
    {
    case 0:
        settings.player_color = Color::White;
        break;
    case 1:
        settings.player_color = Color::Black;
        break;
    case 2:
        settings.player_color = std::rand() & 1 ? Color::White : Color::Black;
        break;
    default:
        return settings;
    }

    // Clock
    std::vector<std::string> clock_options = {"No clock", "10+0", "5+0", "1+0", "10+5", "30+10"};
    int clock_option = selectOption(clock_options, active);
    switch (clock_option)
    {
    case 0:
        settings.play_with_clock = false;
        break;
    case 1:
        settings.play_with_clock = true;
        settings.game_time_min = 10;
        settings.extra_time_sec = 0;
        break;
    case 2:
        settings.play_with_clock = true;
        settings.game_time_min = 5;
        settings.extra_time_sec = 0;
        break;
    case 3:
        settings.play_with_clock = true;
        settings.game_time_min = 1;
        settings.extra_time_sec = 0;
        break;
    case 4:
        settings.play_with_clock = true;
        settings.game_time_min = 10;
        settings.extra_time_sec = 5;
        break;
    case 5:
        settings.play_with_clock = true;
        settings.game_time_min = 30;
        settings.extra_time_sec = 10;
        break;
    default:
        return settings;
    }

    // Bot strength
    if (settings.game_mode != GameMode::BotsOffline)
        return settings;

    std::vector<std::string> bot_options = {"Easy", "Medium", "Hard", "Impossible"};
    int bot_option = selectOption(bot_options, active);
    switch (bot_option)
    {
    case 0:
        settings.bot_strength = BotStrength::Easy;
        break;
    case 1:
        settings.bot_strength = BotStrength::Medium;
        break;
    case 2:
        settings.bot_strength = BotStrength::Hard;
        break;
    case 3:
        settings.bot_strength = BotStrength::Impossible;
        break;
    default:
        return settings;
    }
    return settings;
}


PieceType screenSelectPromotion(const std::atomic<bool>& active)
{
    std::vector<std::string> options = {"Queen", "Rook", "Bishop", "Knight"};
    int option = selectOption(options, active);
    switch (option)
    {
    case 0:
        return PieceType::Queen;
    case 1:
        return PieceType::Rook;
    case 2:
        return PieceType::Bishop;
    case 3:
        return PieceType::Knight;
    default:
        return PieceType::Queen;
    }
}


void displayBotPromotion(PieceType piece_type)
{
    std::string piece_string;
    switch (piece_type)
    {
    case PieceType::Queen:
        piece_string = "Queen";
        break;
    case PieceType::Rook:
        piece_string = "Rook";
        break;
    case PieceType::Bishop:
        piece_string = "Bishop";
        break;
    case PieceType::Knight:
        piece_string = "Knight";
        break;
    default:
        return;
    }
    std::vector<std::string> screen {"Bot promoted to", piece_string};
    Hardware::get().reserveScreen(true);
    Hardware::get().setScreen(screen, 1);
}