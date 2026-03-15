// animations.cpp
#include "animations.hpp"


std::string playAnimations(std::atomic<bool>& stop, const std::vector<Animation>& animations)
{
    for (auto& animation : animations)
    {
        if (animation.display_time_ms <= 0)
        {
            return "error, display time must be greater than 0";
        }
        Hardware::get().setLed(animation.leds);
        for (int _ = 0; _ < animation.display_time_ms; _++)
        {
            if (stop.load())
            {
                Hardware::get().clearLed();
                return "error, stopped";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    Hardware::get().clearLed();
    return "ok";
}


void playWinnerAnimation(const ChessGame& game, int anim_length, int time)
{
    std::vector<Animation> animations;
    animations.reserve(anim_length);

    int white_king = -1;
    int black_king = -1;
    for (int i = 0; i < game.board.size(); i++)
    {
        if (game.board[i] == Piece(Color::White, PieceType::King))
            white_king = i;
        else if (game.board[i] == Piece(Color::Black, PieceType::King))
            black_king = i;
    }
    if (white_king < 0 || black_king < 0)
    {
        return;
    }

    int winner_king, loser_king;
    if (game.winner == Winner::Nil)
        return;
    if (game.winner == Winner::White)
    {
        winner_king = white_king;
        loser_king = black_king;
    }
    else if (game.winner == Winner::Black)
    {
        winner_king = black_king;
        loser_king = white_king;
    }

    for (int i = 0; i < anim_length; i++)
    {
        Animation animation;
        animation.display_time_ms = time;
        for (int j = 0; j < animation.leds.size(); j++)
        {
            if (j % 2 == 0 && i % 2 == 0 || j % 2 != 0 && i % 2 != 0)
                animation.leds[j] = LedColor(0, 0, 255);
            else
                animation.leds[j] = LedColor(0, 0, 0);
        }
        if (game.winner != Winner::Draw)
        {
            animation.leds.at(winner_king) = LedColor(0, 255, 0);
            animation.leds.at(loser_king) = LedColor(255, 0, 0);
        }
        animations.push_back(animation);
    }
    std::atomic<bool> stop{false};
    playAnimations(stop, animations);
}


void playStartUpAnimation(int time)
{
    const Animation ring1 = {{
        LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255),
        LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255)
    }, time};

    const Animation ring2 = {{
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    }, time};

    const Animation ring3 = {{
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    }, time};


    const Animation ring4 = {{
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 255), LedColor(0, 0, 255), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0),
        LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0), LedColor(0, 0, 0)
    }, time};
    std::vector<Animation> animations = {ring1, ring2, ring3, ring4, ring3, ring2, ring1};

    std::atomic<bool> stop{false};
    playAnimations(stop, animations);
}