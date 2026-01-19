// extract_make_move.hpp
#include "extract_make_move.hpp"


ChessGame getChessGame(const std::string& request)
{
    std::string board_str = extractValue(request, "board");
    std::string castling_str = extractValue(request, "castling");
    int en_passant;
    try
    {
        en_passant = stoi(extractValue(request, "en_passant"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, en_passant must be a valid integer");
    }
    std::string player_turn = extractValue(extractValue(request, "clock"), "run_down");

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
    make_move.lifted_square_color = getLedColor(extractValue(request, "lifted_square_color"));
    make_move.legal_moves_color = getLedColor(extractValue(request, "legal_move_color"));
    make_move.illegal_moves_color = getLedColor(extractValue(request, "illegal_move_color"));
    make_move.timeout = extractTimeOut(request);

    std::string past_move_str = extractValue(request, "past_move");
    make_move.past_move_color = getLedColor(extractValue(past_move_str, "color"));
    make_move.past_move_from = stoi(extractValue(past_move_str, "from"));
    make_move.past_move_to = stoi(extractValue(past_move_str, "to"));

    if (make_move.past_move_from < -1 || make_move.past_move_from >= 64) throw std::runtime_error("Error, Invalid past move from value");
    if (make_move.past_move_to < -1 || make_move.past_move_to >= 64) throw std::runtime_error("Error, Invalid past move to value");

    return make_move;
}
