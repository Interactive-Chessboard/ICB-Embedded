// make_move.cpp
#include "make_move.hpp"


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

    // Fix for bit mask
    for (int i = 0; i < 4; i++)
    {
        game.castle.at(0) = castling_str.at(0);
    }

    game.en_passant = en_passant;
    game.player_turn = color;
    return game;
}


uint64_t getGameBitBoard(ChessGame game)
{
    uint64_t bit_board = 0;
    for (int i = 0; i < 64; i++)
    {
        if (game.board.at(i) != Piece())
            bit_board |= (1ULL << i);
    }
    return bit_board;
}


int makeMoveTic(ChessGame game, std::vector<Move> moves,
                LedColor old_move_color, LedColor lifted_square_color, LedColor legal_moves_color,
                LedColor illegal_moves_color, int past_move_from, int past_move_to)
{

}


Move makeMove(std::atomic<bool>& end_task_flag, int timeout, ChessGame game, std::vector<Move> moves,
              LedColor old_move_color, LedColor lifted_square_color, LedColor legal_moves_color,
              LedColor illegal_moves_color, int past_move_from, int past_move_to)
{
    uint64_t original_bit_board = getGameBitBoard(game);
    uint64_t current_bit_board = original_bit_board;

    while (!end_task_flag.load() && timeout < 0)
    {
        uint64_t before_tic = Board::getBoardArr();
        int index = -1;
        if (current_bit_board != before_tic)
        {
            index = makeMoveTic(game, moves, old_move_color, lifted_square_color,
                        legal_moves_color, illegal_moves_color, past_move_from, past_move_to);
            if (index >= 0)
                return moves.at(index);
            current_bit_board = before_tic;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timeout--;
    }
    throw std::runtime_error("Error, timeout reached or end task called");
}


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    ChessGame game;
    LedColor old_move_color, lifted_square_color, legal_moves_color, illegal_moves_color;
    int past_move_from, past_move_to, timeout;
    try
    {
        game = getChessGame(request);
        setClockSettings(std::ref(clock_settings), request);

        old_move_color = getColor(extract_value(extract_value(request, "old_move"), "color"));
        lifted_square_color = getColor(extract_value(request, "lifted_square_color"));
        legal_moves_color = getColor(extract_value(request, "legal_moves_color"));
        illegal_moves_color = getColor(extract_value(request, "illegal_moves_color"));
        past_move_from = stoi(extract_value(extract_value(request, "old_move"), "from"));
        past_move_to = stoi(extract_value(extract_value(request, "old_move"), "to"));
        timeout = extractTimeOut(request) * 100;
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }
    if (past_move_from < -1 || past_move_from >= 64) return "Error, Invalid past move from value";
    if (past_move_to < -1 || past_move_to >= 64) return "Error, Invalid past move to value";

    std::vector<Move> moves = Chess::generateLegalMoves(game);
    Move move_made;
    try
    {
        move_made = makeMove(std::ref(end_task_flag), timeout, game, moves, old_move_color, lifted_square_color,
                              legal_moves_color, illegal_moves_color, past_move_from, past_move_to);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    return "ok, \"move_from\": " + std::to_string(move_made.from_square) +
           ", \"move_to\": " + std::to_string(move_made.to_square) + "\"";
}