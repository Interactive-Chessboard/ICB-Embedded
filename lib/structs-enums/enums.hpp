// enums.hpp
#pragma once

enum class GameMode {
    Online,
    MultiplayerOffline,
    BotsOffline
};


enum class Color {
    White,
    Black,
    Nil
};

enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Nil
};


enum class Winner {
    White,
    Black,
    Draw,
    Nil
};