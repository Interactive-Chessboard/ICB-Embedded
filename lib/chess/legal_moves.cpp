// legal_moves.cpp
#include "chess.hpp"

std::vector<Move> whitePawnMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    // Invalid
    if (index >= 56 && index <= 63)
    {
        return moves;
    }

    // Double move
    if (index >= 8 && index <= 15 && board[index + 8].piece_type == PieceType::Nil && board[index + 16].piece_type == PieceType::Nil)
    {
        moves.push_back(Move(index, index + 16));
    }

    // Move up
    if (board[index + 8].piece_type == PieceType::Nil)
    {
        moves.push_back(Move(index, index + 8));
    }

    // Diagonal left
    if ((index + 7) % 8 != 7 && board[index + 7].color == Color::Black)
    {
        moves.push_back(Move(index, index + 7));
    }

    // Diagonal right
    if ((index + 9) % 8 != 0 && board[index + 9].color == Color::Black)
    {
        moves.push_back(Move(index, index + 9));
    }

    // Promotion
    if (index >= 48 && index <= 55)
    {
        std::vector<Move> new_moves;
        new_moves.reserve(4);
        for (auto move : moves)
        {
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::White, PieceType::Queen)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::White, PieceType::Knight)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::White, PieceType::Bishop)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::White, PieceType::Rook)));
        }
        return new_moves;
    }
    return moves;
}

std::vector<Move> blackPawnMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    // Invalid
    if (index >= 0 && index <= 7)
    {
        return moves;
    }

    // Double move
    if (index >= 48 && index <= 55 && board[index - 8].piece_type == PieceType::Nil && board[index - 16].piece_type == PieceType::Nil)
    {
        moves.push_back(Move(index, index - 16));
    }

    // Move up
    if (board[index - 8].piece_type == PieceType::Nil)
    {
        moves.push_back(Move(index, index - 8));
    }

    // Diagonal right
    if ((index - 7) % 8 != 0 && board[index - 7].color == Color::White)
    {
        moves.push_back(Move(index, index - 7));
    }

    // Diagonal left
    if ((index - 9) % 8 != 7 && board[index - 9].color == Color::White)
    {
        moves.push_back(Move(index, index - 9));
    }

    // Promotion
    if (index >= 8 && index <= 15)
    {
        std::vector<Move> new_moves;
        moves.reserve(4);
        for (auto move : moves)
        {
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::Black, PieceType::Queen)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::Black, PieceType::Knight)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::Black, PieceType::Bishop)));
            new_moves.push_back(Move(move.from_square, move.to_square, Piece(Color::Black, PieceType::Rook)));
        }
        return new_moves;
    }
    return moves;
}

std::vector<Move> knightMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    Color piece_color = board[index].color;

    int knight_moves_left[4] = {index - 17, index - 10, index + 6, index + 15}; // kight moves (not magic numbers)
    int knight_moves_right[4] = {index - 15, index - 6, index + 10, index + 17};
    for (int i = 0; i < 4; i++)
    {
        if (knight_moves_left[i] < 64 && knight_moves_left[i] >= 0 && 
            index % 8 > knight_moves_left[i] % 8 && board[knight_moves_left[i]].color != piece_color)
        {
            moves.push_back(Move(index, knight_moves_left[i]));
        }
        if (knight_moves_right[i] < 64 && knight_moves_right[i] >= 0 && 
            index % 8 < knight_moves_right[i] % 8 && board[knight_moves_right[i]].color != piece_color)
        {
            moves.push_back(Move(index, knight_moves_right[i]));
        }
    }
    return moves;
}


std::vector<Move> bishopMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    Color piece_color = board[index].color;

    // bottom left
    int bottom_left_index = index - 9;
    while (bottom_left_index >= 0 && bottom_left_index % 8 != 7)
    {
        if (board[bottom_left_index].color == Color::Nil)
        {
            moves.push_back(Move(index, bottom_left_index));
        }
        else if (board[bottom_left_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, bottom_left_index));
            break;
        }
        bottom_left_index -= 9;
    }

    // bottom right
    int bottom_right_index = index - 7;
    while (bottom_right_index >= 0 && bottom_right_index % 8 != 0)
    {
        if (board[bottom_right_index].color == Color::Nil)
        {
            moves.push_back(Move(index, bottom_right_index));
        }
        else if (board[bottom_right_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, bottom_right_index));
            break;
        }
        bottom_right_index -= 7;
    }

    // up left
    int up_left_index = index + 7;
    while (up_left_index < 64 && up_left_index % 8 != 7)
    {
        if (board[up_left_index].color == Color::Nil)
        {
            moves.push_back(Move(index, up_left_index));
        }
        else if (board[up_left_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, up_left_index));
            break;
        }
        up_left_index += 7;
    }

    // up right
    int up_right_index = index + 9;
    while (up_right_index < 64 && up_right_index % 8 != 0)
    {
        if (board[up_right_index].color == Color::Nil)
        {
            moves.push_back(Move(index, up_right_index));
        }
        else if (board[up_right_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, up_right_index));
            break;
        }
        up_right_index += 9;
    }
    return moves;
}


std::vector<Move> rookMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    Color piece_color = board[index].color;

    // left
    int left_index = index - 1;
    while (left_index >= 0 && left_index % 8 != 7)
    {
        if (board[left_index].color == Color::Nil)
        {
            moves.push_back(Move(index, left_index));
        }
        else if (board[left_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, left_index));
            break;
        }
        left_index--;
    }

    // right
    int right_index = index + 1;
    while (right_index < 64 && right_index % 8 != 0)
    {
        if (board[right_index].color == Color::Nil)
        {
            moves.push_back(Move(index, right_index));
        }
        else if (board[right_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, right_index));
            break;
        }
        right_index++;
    }

    // bottom
    int bottom_index = index - 8;
    while (bottom_index >= 0)
    {
        if (board[bottom_index].color == Color::Nil)
        {
            moves.push_back(Move(index, bottom_index));
        }
        else if (board[bottom_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, bottom_index));
            break;
        }
        bottom_index -= 8;
    }

    // up
    int up_index = index + 8;
    while (up_index < 64)
    {
        if (board[up_index].color == Color::Nil)
        {
            moves.push_back(Move(index, up_index));
        }
        else if (board[up_index].color == piece_color)
        {
            break;
        }
        else
        {
            moves.push_back(Move(index, up_index));
            break;
        }
        up_index += 8;
    }
    return moves;
}

std::vector<Move> kingMoves(const Piece board[64], int index)
{
    std::vector<Move> moves;
    moves.reserve(10);
    Color piece_color = board[index].color;

    int king_moves_left[3] = {index - 9, index - 1, index + 7}; // king moves (not magic numbers)
    int king_moves_right[3] = {index - 7, index + 1, index + 9};
    int king_moves[2] = {index - 8, index + 8};
    for (int i = 0; i < 3; i++)
    {
        if (king_moves_left[i] < 64 && king_moves_left[i] >= 0 && 
            index % 8 > king_moves_left[i] % 8 && board[king_moves_left[i]].color != piece_color)
        {
            moves.push_back(Move(index, king_moves_left[i]));
        }
        if (king_moves_right[i] < 64 && king_moves_right[i] >= 0 && 
            index % 8 < king_moves_right[i] % 8 && board[king_moves_right[i]].color != piece_color)
        {
            moves.push_back(Move(index, king_moves_right[i]));
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (king_moves[i] < 64 && king_moves[i] >= 0 && 
            board[king_moves[i]].color != piece_color)
        {
            moves.push_back(Move(index, king_moves[i]));
        }
    }
    return moves;
}

Move updateBoard(ChessGame chess_game, Move move)
{
    // Make move
    if (move.promotion != Piece())
    {
        chess_game.board[move.to_square] = move.promotion;
    }
    else 
    {
        chess_game.board[move.to_square] = chess_game.board[move.from_square];
    }  
    chess_game.board[move.from_square] = Piece();
    
    // En passant flags
    chess_game.en_passant = -1;
    if (chess_game.player_turn == Color::White && (move.to_square - move.from_square) == 16 && 
        chess_game.board[move.to_square] == Piece(Color::White, PieceType::Pawn) &&
        ((chess_game.board[move.to_square - 1] == Piece(Color::Black, PieceType::Pawn) && move.to_square - 1 >= 24) ||
        (chess_game.board[move.to_square + 1] == Piece(Color::Black, PieceType::Pawn) && move.to_square + 1 < 32)))
    {
        chess_game.en_passant = move.to_square - 8;
    }

    if (chess_game.player_turn == Color::Black && (move.to_square - move.from_square) == -16 && 
        chess_game.board[move.to_square] == Piece(Color::Black, PieceType::Pawn) &&
        ((chess_game.board[move.to_square - 1] == Piece(Color::White, PieceType::Pawn) && move.to_square - 1 >= 32) ||
        (chess_game.board[move.to_square + 1] == Piece(Color::White, PieceType::Pawn) && move.to_square + 1 < 39)))
    {
        chess_game.en_passant = move.to_square + 8;
    }

    // Castle flags
    if (chess_game.castle[0] == 'K' && 
        (chess_game.board[4] != Piece(Color::White, PieceType::King) ||
        chess_game.board[7] != Piece(Color::White, PieceType::Rook)))
    {
        chess_game.castle[0] = ' ';
    }

    if (chess_game.castle[1] == 'Q' && 
        (chess_game.board[4] != Piece(Color::White, PieceType::King) ||
        chess_game.board[0] != Piece(Color::White, PieceType::Rook)))
    {
        chess_game.castle[1] = ' ';
    }

    if (chess_game.castle[2] == 'k' && 
        (chess_game.board[60] != Piece(Color::Black, PieceType::King) ||
        chess_game.board[63] != Piece(Color::Black, PieceType::Rook)))
    {
        chess_game.castle[2] = ' ';
    }

    if (chess_game.castle[3] == 'q' && 
        (chess_game.board[60] != Piece(Color::Black, PieceType::King) ||
        chess_game.board[56] != Piece(Color::Black, PieceType::Rook)))
    {
        chess_game.castle[3] = ' ';
    }

    move.chess_game = chess_game;
    return move;
}

std::vector<Move> generatePseudoLegalMoves(ChessGame chess_game)
{
    std::vector<Move> pseudo_moves;
    pseudo_moves.reserve(50);

    // Normal moves
    for (int i = 0; i < 64; i++)
    {  
        Piece piece = chess_game.board[i];
        if (chess_game.player_turn != piece.color)
        {
            continue;
        }
        std::vector<Move> new_moves;
        new_moves.reserve(50);
        std::vector<Move> new_moves2;
        new_moves2.reserve(10);
        switch (piece.piece_type) 
        {
        case PieceType::Pawn:
            new_moves = piece.color == Color::White ? whitePawnMoves(chess_game.board, i) : blackPawnMoves(chess_game.board, i);
            break;
        case PieceType::Knight:
            new_moves = knightMoves(chess_game.board, i);
            break;
        case PieceType::Bishop:
            new_moves = bishopMoves(chess_game.board, i);
            break;
        case PieceType::Rook:
            new_moves = rookMoves(chess_game.board, i);
            break;
        case PieceType::Queen:
            new_moves = rookMoves(chess_game.board, i);
            new_moves2 = bishopMoves(chess_game.board, i);
            new_moves.insert(new_moves.end(), new_moves2.begin(), new_moves2.end());
            break;
        case PieceType::King:
            new_moves = kingMoves(chess_game.board, i);
            break;
        default:
            break;
        }
        pseudo_moves.insert(pseudo_moves.end(), new_moves.begin(), new_moves.end());
    }

    std::vector<Move> complete_pseudo_moves;
    complete_pseudo_moves.reserve(50);
    for (auto move : pseudo_moves)
    {
        Move update_move = updateBoard(chess_game, move);
        complete_pseudo_moves.push_back(update_move);
    }

    return complete_pseudo_moves;
}


std::vector<Move> enPassantMoves(ChessGame chess_game)
{
    std::vector<Move> moves;
    moves.reserve(1);
    if (chess_game.player_turn == Color::White)
    {
        if (chess_game.board[chess_game.en_passant - 7] == Piece(Color::White, PieceType::Pawn))
        {
            Move move = Move(chess_game.en_passant - 7, chess_game.en_passant);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.board[chess_game.en_passant] = Piece(Color::White, PieceType::Pawn);
            move.chess_game.board[chess_game.en_passant - 7] = Piece();
            move.chess_game.board[chess_game.en_passant - 8] = Piece();
            moves.push_back(move);
        }
        if (chess_game.board[chess_game.en_passant - 9] == Piece(Color::White, PieceType::Pawn))
        {
            Move move = Move(chess_game.en_passant - 9, chess_game.en_passant);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.board[chess_game.en_passant] = Piece(Color::White, PieceType::Pawn);
            move.chess_game.board[chess_game.en_passant - 9] = Piece();
            move.chess_game.board[chess_game.en_passant - 8] = Piece();
            moves.push_back(move);
        }
    }
    else
    {
        if (chess_game.board[chess_game.en_passant + 7] == Piece(Color::Black, PieceType::Pawn))
        {
            Move move = Move(chess_game.en_passant + 7, chess_game.en_passant);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.board[chess_game.en_passant] = Piece(Color::Black, PieceType::Pawn);
            move.chess_game.board[chess_game.en_passant + 7] = Piece();
            move.chess_game.board[chess_game.en_passant + 8] = Piece();
            moves.push_back(move);
        }
        if (chess_game.board[chess_game.en_passant + 9] == Piece(Color::Black, PieceType::Pawn))
        {
            Move move = Move(chess_game.en_passant + 9, chess_game.en_passant);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.board[chess_game.en_passant] = Piece(Color::Black, PieceType::Pawn);
            move.chess_game.board[chess_game.en_passant + 9] = Piece();
            move.chess_game.board[chess_game.en_passant + 8] = Piece();
            moves.push_back(move);
        }
    }
    return moves;
}


std::vector<Move> castleMoves(ChessGame chess_game)
{
    std::vector<Move> moves;
    moves.reserve(2);

    // King side castle white
    if (chess_game.player_turn == Color::White && chess_game.castle[0] == 'K' &&
        chess_game.board[5] == Piece() && chess_game.board[6] == Piece() &&
        !Chess::isInCheck(chess_game, 4) && !Chess::isInCheck(chess_game, 5) && !Chess::isInCheck(chess_game, 6))
        {
            Move move = Move(4, 6);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.castle[0] = ' ';
            move.chess_game.castle[1] = ' ';
            move.chess_game.player_turn = Color::Black;
            move.chess_game.board[4] = Piece();
            move.chess_game.board[5] = Piece(Color::White, PieceType::Rook);
            move.chess_game.board[6] = Piece(Color::White, PieceType::King);
            move.chess_game.board[7] = Piece();
            moves.push_back(move);
        }
    
    // Queen side castle black
    if (chess_game.player_turn == Color::White && chess_game.castle[1] == 'Q' &&
        chess_game.board[3] == Piece() && chess_game.board[2] == Piece() && chess_game.board[1] == Piece() &&
        !Chess::isInCheck(chess_game, 4) && !Chess::isInCheck(chess_game, 3) && !Chess::isInCheck(chess_game, 2))
        {
            Move move = Move(4, 2);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.castle[0] = ' ';
            move.chess_game.castle[1] = ' ';
            move.chess_game.player_turn = Color::Black;
            move.chess_game.board[0] = Piece();
            move.chess_game.board[1] = Piece();
            move.chess_game.board[2] = Piece(Color::White, PieceType::King);
            move.chess_game.board[3] = Piece(Color::White, PieceType::Rook);
            move.chess_game.board[4] = Piece();
            moves.push_back(move);
        }
    
    // King side castle black
    if (chess_game.player_turn == Color::Black && chess_game.castle[2] == 'k' &&
        chess_game.board[61] == Piece() && chess_game.board[62] == Piece() &&
        !Chess::isInCheck(chess_game, 60) && !Chess::isInCheck(chess_game, 61) && !Chess::isInCheck(chess_game, 62))
        {
            Move move = Move(60, 62);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.castle[2] = ' ';
            move.chess_game.castle[3] = ' ';
            move.chess_game.player_turn = Color::White;
            move.chess_game.board[60] = Piece();
            move.chess_game.board[61] = Piece(Color::Black, PieceType::Rook);
            move.chess_game.board[62] = Piece(Color::Black, PieceType::King);
            move.chess_game.board[63] = Piece();
            moves.push_back(move);
        }
    
    // Queen side castle black
    if (chess_game.player_turn == Color::Black && chess_game.castle[3] == 'q' &&
        chess_game.board[57] == Piece() && chess_game.board[58] == Piece() && chess_game.board[59] == Piece() &&
        !Chess::isInCheck(chess_game, 58) && !Chess::isInCheck(chess_game, 59) && !Chess::isInCheck(chess_game, 60))
        {
           Move move = Move(60, 58);
            move.chess_game = chess_game;
            move.chess_game.en_passant = -1;
            move.chess_game.castle[2] = ' ';
            move.chess_game.castle[3] = ' ';
            move.chess_game.player_turn = Color::White;
            move.chess_game.board[56] = Piece();
            move.chess_game.board[57] = Piece();
            move.chess_game.board[58] = Piece(Color::Black, PieceType::King);
            move.chess_game.board[59] = Piece(Color::Black, PieceType::Rook);
            move.chess_game.board[60] = Piece();
            moves.push_back(move);
        }
    return moves;
}


std::vector<Move> Chess::generateLegalMoves(ChessGame chess_game)
{

    // Pseudo legal moves
    std::vector<Move> pseudo_moves = generatePseudoLegalMoves(chess_game);
    pseudo_moves.reserve(50);

    // En passant
    std::vector<Move> en_passant_moves;
    en_passant_moves.reserve(1);
    if (chess_game.en_passant != -1)
    {
        en_passant_moves = enPassantMoves(chess_game);
    }
    pseudo_moves.insert(pseudo_moves.end(), en_passant_moves.begin(), en_passant_moves.end());

    // Add Castle Moves
    std::vector<Move> moves = castleMoves(chess_game);

    // Verify move is legal
    for (auto move : pseudo_moves)
    {
        if (!Chess::isInCheck(move.chess_game))
        {
            // Update player turn flag at the end
            move.chess_game.player_turn = (chess_game.player_turn == Color::White) ? Color::Black : Color::White;
            moves.push_back(move);
        }
    }
    
    return moves;
}