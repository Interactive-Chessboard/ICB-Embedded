// screen_selection
#include "screen_selection.hpp"


int selectOption(std::vector<std::string> options, const std::atomic<bool>& active)
{
    int selected = 0;
    while(active.load())
    {
        Hardware::get().setOptions(options, selected);
        if (Hardware::get().detectStartClick())
            return selected;
        if (Hardware::get().detectSelectClick())
        {
            if (selected >= options.size())
                selected = 0;
            else
                selected++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}


Settings getGameSettings()
{
    Settings settings;
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