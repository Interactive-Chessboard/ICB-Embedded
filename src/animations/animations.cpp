// animations.cpp
#include "animations.hpp"


std::string playAnimations(std::atomic<bool>& stop, const std::vector<Animation>& animations)
{
    for (auto& animation : animations)
    {
        Hardware::get().setLed(animation.leds);
        for (int _ = 0; _ < animation.display_time_ms; _++)
        {
            if (stop.load())
            {
                Hardware::get().clearLed();
                return "Error, stopped";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    Hardware::get().clearLed();
    return "ok";
}


void playWinnerAnimation(ChessGame game)
{
    int anim_length;
    std::vector<Animation> animations;
    animations.reserve(anim_length);

    int white_king, black_king;
    for (int i = 0; i < game.board.size(); i++)
    {
        if (game.board[i] == Piece(Color::White, PieceType::King))
            white_king = i;
        else if (game.board[i] == Piece(Color::Black, PieceType::King))
            black_king = i;
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
        animation.display_time_ms = 1000;
        for (int j = 0; j < animation.leds.size(); i++)
        {
            if (j % 2 == 0 && i % 2 == 0 || j % 2 != 0 && i % 2 != 0)
                animation.leds[j] = (0, 0, 255);
            else
                animation.leds[j] = (0, 0, 0);
        }
        if (game.winner != Winner::Draw)
        {
            animation.leds.at(winner_king) = (0, 255, 0);
            animation.leds.at(loser_king) = (255, 0, 0);
        }
    }

    std::atomic<bool> stop{false};
    playAnimations(stop, animations);
}

const Animation ring1 = {{
    (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255),
    (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255)
}, 1000};

const Animation ring2 = {{
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)
}, 1000};

const Animation ring3 = {{
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0),(0, 0, 255), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 0), (0, 0, 0),(0, 0, 255), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 255), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)
}, 1000};


const Animation ring4 = {{
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 255), (0, 0, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)
}, 1000};


void playStartUpAnimation()
{
    std::vector<Animation> animations = {ring1, ring2, ring3, ring4, ring3, ring2, ring1};

    std::atomic<bool> stop{false};
    playAnimations(stop, animations);
}