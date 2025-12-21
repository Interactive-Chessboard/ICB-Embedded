// extract_make_move.hpp
#include "extract_make_move.hpp"


ChessGame getChessGame(const std::string& request)
{
    std::string board_str = extract_value(request, "board");
    std::string castling_str = extract_value(request, "castling");
    int en_passant;
    try
    {
        en_passant = stoi(extract_value(request, "en_passant"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, en_passant must be a valid integer");
    }
    std::string player_turn = extract_value(extract_value(request, "clock"), "run_down");

    if (board_str.length() != 64) throw std::runtime_error("Error, board must be 64 characters long");
    if (castling_str.length() != 4) throw std::runtime_error("Error, castling must be 4 characters long");
    if (en_passant < -1 || en_passant >= 64) throw std::runtime_error("Error, Invalid en passant value");
    Color color;
    if (player_turn == "w")
        color = Color::White;
    else if (player_turn == "b")
        color = Color::Black;
    else
        throw std::runtime_error("Error, invalid clock color");

    ChessGame game;
    for (int i = 0 ; i < 64; i++)
    {
        switch (board_str.at(i))
        {
        case '.':
            game.board.at(i) = Piece();
            break;
        case 'P':
            game.board.at(i) = Piece(Color::White, PieceType::Pawn);
            break;
        case 'p':
            game.board.at(i) = Piece(Color::Black, PieceType::Pawn);
            break;
        case 'R':
            game.board.at(i) = Piece(Color::White, PieceType::Rook);
            break;
        case 'r':
            game.board.at(i) = Piece(Color::Black, PieceType::Rook);
            break;
        case 'N':
            game.board.at(i) = Piece(Color::White, PieceType::Knight);
            break;
        case 'n':
            game.board.at(i) = Piece(Color::Black, PieceType::Knight);
            break;
        case 'B':
            game.board.at(i) = Piece(Color::White, PieceType::Bishop);
            break;
        case 'b':
            game.board.at(i) = Piece(Color::Black, PieceType::Bishop);
            break;
        case 'Q':
            game.board.at(i) = Piece(Color::White, PieceType::Queen);
            break;
        case 'q':
            game.board.at(i) = Piece(Color::Black, PieceType::Queen);
            break;
        case 'K':
            game.board.at(i) = Piece(Color::White, PieceType::King);
            break;
        case 'k':
            game.board.at(i) = Piece(Color::Black, PieceType::King);
            break;
        default:
            throw std::runtime_error("Error, invalid piece");
            break;
        }
    }

    // TO DO: Change castling to bit mask
    for (int i = 0; i < 4; i++)
    {
        game.castle.at(0) = castling_str.at(0);
    }

    game.en_passant = en_passant;
    game.player_turn = color;
    return game;
}


ExtractMakeMove extractMakeMove(const std::string& request)
{
    ExtractMakeMove make_move;
    make_move.game = getChessGame(request);
    make_move.past_move_color = getColor(extract_value(extract_value(request, "past_move"), "color"));
    make_move.lifted_square_color = getColor(extract_value(request, "lifted_square_color"));
    make_move.legal_moves_color = getColor(extract_value(request, "legal_moves_color"));
    make_move.illegal_moves_color = getColor(extract_value(request, "illegal_moves_color"));
    make_move.past_move_from = stoi(extract_value(extract_value(request, "past_move"), "from"));
    make_move.past_move_to = stoi(extract_value(extract_value(request, "past_move"), "to"));
    make_move.timeout = extractTimeOut(request) * 100;

    if (make_move.past_move_from < -1 || make_move.past_move_from >= 64) throw std::runtime_error("Error, Invalid past move from value");
    if (make_move.past_move_to < -1 || make_move.past_move_to >= 64) throw std::runtime_error("Error, Invalid past move to value");
}