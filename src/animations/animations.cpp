// animations.cpp
#include "animations.hpp"


/**
 * @brief Plays a sequence of LED animations in order.
 *
 * Each animation is displayed by sending its LED buffer to the hardware and
 * holding it for `display_time_ms` milliseconds. The function checks once per
 * millisecond whether execution should be stopped.
 *
 * If any animation has a non-positive display time, the function aborts
 * immediately without displaying any animations.
 *
 * If the stop flag is set while an animation is playing, the LEDs are cleared
 * and the function aborts.
 *
 * After all animations are played successfully, the LEDs are cleared.
 *
 * @param stop Atomic flag that can be set to true to interrupt playback.
 * @param animations Vector of animations to play in sequence.
 *
 * @return "ok" if all animations were played successfully.
 * @return "error, display time must be greater than 0" if any animation has
 *         display_time_ms <= 0.
 * @return "error, stopped" if playback was interrupted via the stop flag.
 */
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


/**
 * @brief Plays a winner animation based on the result of a chess game.
 *
 * The animation alternates a blue checkerboard pattern for a given number of
 * frames. If the game has a decisive winner, the winning king's square is shown
 * in green and the losing king's square in red for every frame.
 *
 * The animation is only played if:
 * - Both white and black kings are present on the board
 * - The game winner is not Winner::Nil
 *
 * If the game is a draw, only the checkerboard animation is shown.
 *
 * Internally, this function constructs a sequence of animations and plays them
 * using playAnimations().
 *
 * @param game The chess game containing board state and winner information.
 * @param anim_length Number of animation frames to generate.
 * @param time Display time in milliseconds for each frame.
 */
void playWinnerAnimation(ChessGame game, int anim_length, int time)
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


/**
 * @brief Plays the startup LED animation.
 *
 * The startup animation consists of a sequence of concentric blue "rings"
 * expanding toward the center of the board and then contracting back outward.
 * The animation order is:
 *
 *   ring1 → ring2 → ring3 → ring4 → ring3 → ring2 → ring1
 *
 * Each frame is displayed for the specified duration.
 *
 * Internally, this function builds a fixed animation sequence and plays it
 * using playAnimations().
 *
 * @param time Display time in milliseconds for each animation frame.
 */
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