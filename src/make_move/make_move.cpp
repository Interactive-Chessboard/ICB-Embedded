// make_move.cpp
#include "make_move.hpp"


std::pair<bool, bool> MakeMove::determineSpecialMoveLift(Move move, int lifted)
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
            current_bit_board == Chess::getGameBitBoard(moves[i].chess_game))
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
    original_bit_board = Chess::getGameBitBoard(game);
    current_bit_board = original_bit_board;
}


Move MakeMove::startOnline(const std::atomic<bool>& end_task_flag)
{
    while (!end_task_flag.load() && timeout > 0)
    {
        uint64_t bit_board_tick = Hardware::get().getBoardArr();
        bool changes = detectChangeTick(bit_board_tick);
        if (changes)
        {
            int move_index = calculateMoveTick();
            if (move_index >= 0)
                return moves.at(move_index);
            std::array<LedColor, 64> led_lights = getBoardLights();
            Hardware::get().setLed(led_lights);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timeout--;
    }
    throw std::runtime_error("Error, timeout reached or end task called");
}


Move MakeMove::returnMove(const std::atomic<bool>& active, int move_index)
{
    Move move = moves.at(move_index);
    if (move.promotion == Piece())
        return move;

    PieceType piece_type = screenSelectPromotion(active);
    for (Move promo_move : moves)
    {
        if (promo_move.from_square == move.from_square &&
            promo_move.to_square == move.to_square &&
            promo_move.promotion.piece_type == piece_type)
            return promo_move;
    }
    return move;
}


Move MakeMove::startOffline(const std::atomic<bool>& active)
{
    while (active.load())
    {
        uint64_t bit_board_tick = Hardware::get().getBoardArr();
        bool changes = detectChangeTick(bit_board_tick);
        if (changes)
        {
            int move_index = calculateMoveTick();
            if (move_index >= 0)
            {
                return returnMove(active, move_index);
            }
            std::array<LedColor, 64> led_lights = getBoardLights();
            Hardware::get().setLed(led_lights);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return Move{};
}
