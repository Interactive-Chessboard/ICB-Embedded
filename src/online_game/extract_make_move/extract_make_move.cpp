// extract_make_move.hpp
#include "extract_make_move.hpp"


ChessGame getChessGame(const std::string& request)
{
    ChessGame game;

    std::string castling_str = extractValue(request, "castling");
    if (castling_str.length() != 4) throw std::runtime_error("Error, castling must be 4 characters long");
    std::array<char, 4> expected = {'K', 'Q', 'k', 'q'};
    for (int i = 0; i < 4; i++)
    {
        char castling = castling_str.at(i);
        if (castling == expected.at(i))
            game.castle.at(i) = castling;
        else if (castling == '.')
            game.castle.at(i) = '.';
        else
            throw std::runtime_error("Error, invalid castling character");
    }

    int en_passant;
    try
    {
        en_passant = stoi(extractValue(request, "en_passant"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, en passant must be a valid integer");
    }
    if (en_passant < -1 || en_passant >= 64) throw std::runtime_error("Error, invalid en passant number");
    game.en_passant = en_passant;

    std::string player_turn = extractValue(extractValue(request, "clock"), "run_down");
    Color color;
    if (player_turn == "w")
        color = Color::White;
    else if (player_turn == "b")
        color = Color::Black;
    else
        throw std::runtime_error("Error, invalid player color");
    game.player_turn = color;

    std::string board_str = extractValue(request, "board");
    if (board_str.length() != 64) throw std::runtime_error("Error, board must be 64 characters long");
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
        }
    }

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
