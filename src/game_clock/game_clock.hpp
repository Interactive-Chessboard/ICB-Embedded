// game_clock.hpp
#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "hardware.hpp"
#include "chess.hpp"


/**
 * @brief Configuration and runtime state for a chess game clock.
 *
 * This structure holds all timing-related data for a two-player chess game,
 * including remaining time for each side, increment settings, and control
 * flags used by the clock thread.
 *
 * All time values are stored in milliseconds.
 *
 * Thread-safety:
 * - Individual fields are atomic to allow concurrent reads/writes.
 * - The mutex is used to synchronize compound operations that must be
 *   performed atomically (e.g., decrementing a clock and applying increment).
 *
 * Typical usage:
 * - A background thread repeatedly calls `tick()` while `active == true`.
 * - The `player_turn` determines which clock is currently running.
 * - When a player's time reaches zero, `active` becomes false.
 */
struct ClockSetting
{
    std::atomic<bool> active{true};
    std::atomic<Color> player_turn{Color::White};
    std::atomic<int> time_white;
    std::atomic<int> time_black;
    std::atomic<int> extra_time;
    std::mutex mtx;
    ClockSetting(int game_time_min, int extra_time_sec)
                : time_white(game_time_min * 60000), time_black(game_time_min * 60000), extra_time(extra_time_sec * 1000) {}
};


/**
 * @brief Starts the background game clock loop.
 *
 * This function runs a timing loop that updates the active player's clock
 * at a fixed interval (1 ms resolution) until termination is requested.
 *
 * Behavior:
 * - Decrements the clock of the player whose turn it is
 * - Applies increment when the turn changes
 * - Stops the clock when either player runs out of time
 * - Periodically updates the hardware display
 *
 * The function is intended to run in its own thread.
 *
 * Thread safety:
 * - Uses atomics for fast concurrent access
 * - Uses the internal mutex to protect compound updates
 *
 * Termination:
 * - The loop exits when `stop_clock_loop` becomes true
 *
 * @param clock_settings Shared clock state structure
 * @param stop_clock_loop Flag used to terminate the loop safely from an external source
 *
 * @note Uses std::chrono::steady_clock to avoid issues with system time changes.
 * @note Sleep scheduling is approximate and depends on OS timing precision.
 */
void startGameClock(ClockSetting&, const std::atomic<bool>&);
