// extract_make_move.hpp
#pragma once
#include "chess.hpp"
#include "hardware.hpp"
#include "../extract_value/extract_value.hpp"


struct ExtractMakeMove
{
    ChessGame game;
    LedColor past_move_color;
    LedColor lifted_square_color;
    LedColor legal_moves_color;
    LedColor illegal_moves_color;
    int past_move_from;
    int past_move_to;
    int timeout;
};


/**
 * @brief Extracts and validates a "make_move" request into a structured ExtractMakeMove object.
 *
 * This function parses a JSON-formatted request string representing a chess "make_move" action.
 * It validates and extracts:
 *  - The current chess game state (board, castling rights, en passant square, active player)
 *  - Past move information (from/to squares and LED color)
 *  - LED colors used to display lifted squares, legal moves, and illegal moves
 *  - The move timeout value
 *
 * Internally, the function performs strict validation and will throw std::runtime_error
 * if any field is malformed or contains invalid values.
 *
 * Validation rules include:
 *  - Castling must be exactly 4 characters long and contain only valid characters
 *    ('K', 'Q', 'k', 'q', or '.')
 *  - The board must be exactly 64 characters long and contain only valid piece symbols
 *  - En passant must be an integer in the range [-1, 63]
 *  - Player turn must be either 'w' (white) or 'b' (black)
 *  - Past move squares must be in the range [-1, 63]
 *
 * @param request A JSON-formatted string representing a "make_move" request.
 *
 * @return ExtractMakeMove A fully populated ExtractMakeMove structure containing the parsed
 *         chess game state, colors, past move data, and timeout.
 *
 * @throws std::runtime_error If any required field is missing, malformed, or contains
 *         invalid values.
 */
ExtractMakeMove extractMakeMove(const std::string& request);
