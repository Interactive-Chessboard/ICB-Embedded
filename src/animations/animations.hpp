// animations.hpp
#pragma once
#include <array>
#include <string>
#include <unordered_set>
#include "hardware.hpp"
#include "chess.hpp"


struct Animation {
    std::array<LedColor, 64> leds;
    int display_time_ms;
};


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
std::string playAnimations(std::atomic<bool>&, const std::vector<Animation>&);


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
void playWinnerAnimation(const ChessGame&, int, int);


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
void playStartUpAnimation(int);
