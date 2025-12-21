// make_move.cpp
#include "make_move.hpp"


std::pair<bool, bool> determineSpecialMoveLift(Move move, int lifted)
{
    // White King castle
    if (lifted == 4 && move.from_square == 4 && move.to_square == 6)
        return {7, 5};
    
    // White Queen castle
    if (lifted == 4 && move.from_square == 4 && move.to_square == 2)
        return {0, 3};

    // Black King Castle
    if (lifted == 60 && move.from_square == 60 && move.to_square == 62)
        return {63, 61};

    // Black Queen Castle
    if (lifted == 60 && move.from_square == 60 && move.to_square == 58)
        return {56, 59};

    int diff = move.to_square - move.from_square;
    // Right en passant both colors
    if (diff == 9 || diff == -7)
        return {move.to_square + 1, -1};

    // Left en passant both colors
    if (diff == 7 || diff == -9)
        return {move.to_square - 1, -1};


    return {-1, -1};
}


bool MakeMove::detectChangeTick(uint64_t tick_bit_board)
{
    uint64_t diff = current_bit_board ^ tick_bit_board;
    if (diff == 0)
        return false;

    while (diff) {
        int index = __builtin_ctzll(diff);
        bool placed_bool = (tick_bit_board >> index) & 1ULL;

        // Valid lifted player pieced
        bool valid_player_lift = false;
        bool valid_opponent_lift = false;
        bool valid_placed_move = false;
        int special_move_lift_index = false;
        int special_move_placed_index = false;
        for (Move move : moves)
        {
            if (!placed_bool && move.from_square == index)
                valid_player_lift = true;
            else if (!placed_bool && move.to_square == index)
                valid_opponent_lift = true;
            else if (placed_bool && move.to_square == index && move.to_square == lifted)
                valid_placed_move;
            else if (lifted == move.from_square && move.special_move)
            {
                std::pair<bool, bool> special_move = determineSpecialMoveLift(move, lifted);
                special_move_lift_index = special_move.first;
                special_move_placed_index = special_move.second;
            }
        }
        
        if (placed_bool)
        {
            // Place piece at a valid position
            if (valid_placed_move)
                placed = index;

            // Place special move
            else if (special_move_placed_index == index)
                placed_special = index;

            // Place back lifted piece
            else if (lifted == index)
                lifted = -1;

            // Place back special move
            else if (lifted_special == index)
                lifted_special = -1;

            // Place back illegally lifted
            else if (illegal_lifted.find(index) != illegal_lifted.end())
                illegal_lifted.erase(index);

            // Else placed illegally
            else
                illegal_lifted.insert(index);
        }
        else
        {
            // Lift first piece if no pieces are lifted
            if (lifted == -1 && valid_player_lift)
                lifted = index;

            // Lift opponent if a piece has been lifted
            else if (lifted != -1 && valid_opponent_lift)
                lifted_opponent = index;

            // Lift special move
            else if (special_move_lift_index == index)
                special_move_lift_index = index;

            // Lift from special placed
            else if (placed_special == index)
                placed_special = -1;

            // Lifted from illegal placed
            else if (illegal_placed.find(index) != illegal_placed.end())
                illegal_placed.erase(index);

            // If previous conditions aren't met, lift is illegal
            else
                illegal_lifted.insert(index);
        }

        diff &= diff - 1;
    }
    current_bit_board = tick_bit_board;
    return true;
}


int MakeMove::calculateMoveTick()
{
    // No current illegal moves
    if (!illegal_lifted.empty() || !illegal_placed.empty())
        return -1;

    for (int i = 0; i < moves.size(); i++)
    {
        // Lifted and placed piece correspond to a move and the bit board corresponds to that moves result as well
        if (moves[i].from_square == lifted && moves[i].to_square == placed &&
            current_bit_board == getGameBitBoard(moves[i].chess_game))
            return i;
    }
    return -1;
}


std::array<LedColor, 64> MakeMove::getBoardLights()
{
    std::array<LedColor, 64> lights;
    // Light past move first (can be overriden)
    lights.at(past_move_from) = past_move_color;
    lights.at(past_move_to) = past_move_color;

    // Light illegal moves
    for (int val : illegal_lifted)
        lights.at(val) = illegal_moves_color;
    for (int val : illegal_placed)
        lights.at(val) = illegal_moves_color;

    // Move is almost completed
    if (lifted != -1 && lifted_opponent != -1 && placed == -1)
    {
        lights.at(lifted) = lifted_square_color;
        lights.at(lifted_opponent) = legal_moves_color;
    }
    // Display legal moves
    else if (lifted != -1 && placed == -1)
    {
        lights.at(lifted) = lifted_square_color;
        for (Move move : moves)
        {
            if (move.from_square == lifted)
                lights.at(move.to_square) = legal_moves_color;
        }
    }
    return lights;
}


void MakeMove::construct()
{
    moves = Chess::generateLegalMoves(game);
    original_bit_board = getGameBitBoard(game);
    current_bit_board = original_bit_board;

}

uint64_t MakeMove::getGameBitBoard(ChessGame game)
{
    uint64_t bit_board = 0;
    for (int i = 0; i < 64; i++)
    {
        if (game.board.at(i) != Piece())
            bit_board |= (1ULL << i);
    }
    return bit_board;
}


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


Move detectMakeMove(std::atomic<bool>& end_task_flag, int timeout, ChessGame game, std::vector<Move> moves,
              LedColor past_move_color, LedColor lifted_square_color, LedColor legal_moves_color,
              LedColor illegal_moves_color, int past_move_from, int past_move_to)
{
    MakeMove make_move(game, past_move_from, past_move_to, past_move_color, lifted_square_color, legal_moves_color, illegal_moves_color);

    while (!end_task_flag.load() && timeout > 0)
    {
        uint64_t bit_board_tick = Board::getBoardArr();
        bool changes = make_move.detectChangeTick(bit_board_tick);
        if (changes)
        {
            int move_index = make_move.calculateMoveTick();
            if (move_index >= 0)
                return moves.at(move_index);
            std::array<LedColor, 64> led_lights = make_move.getBoardLights();
            Board::setLed(led_lights);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timeout--;
    }
    throw std::runtime_error("Error, timeout reached or end task called");
}


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    ChessGame game;
    LedColor past_move_color, lifted_square_color, legal_moves_color, illegal_moves_color;
    int past_move_from, past_move_to, timeout;
    try
    {
        game = getChessGame(request);
        setClockSettings(std::ref(clock_settings), request);

        past_move_color = getColor(extract_value(extract_value(request, "past_move"), "color"));
        lifted_square_color = getColor(extract_value(request, "lifted_square_color"));
        legal_moves_color = getColor(extract_value(request, "legal_moves_color"));
        illegal_moves_color = getColor(extract_value(request, "illegal_moves_color"));
        past_move_from = stoi(extract_value(extract_value(request, "past_move"), "from"));
        past_move_to = stoi(extract_value(extract_value(request, "past_move"), "to"));
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
        move_made = detectMakeMove(std::ref(end_task_flag), timeout, game, moves, past_move_color, lifted_square_color,
                              legal_moves_color, illegal_moves_color, past_move_from, past_move_to);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    return "ok, \"move_from\": " + std::to_string(move_made.from_square) +
           ", \"move_to\": " + std::to_string(move_made.to_square) + "\"";
}