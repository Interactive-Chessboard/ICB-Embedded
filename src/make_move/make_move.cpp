// make_move.cpp
#include "make_move.hpp"
#include <iostream>


void MakeMove::initialize()
{
    if (moves.size() == 0)
        moves = Chess::generateLegalMoves(game);

    Color opponent_color = game.player_turn == Color::White ? Color::Black : Color::White;

    for (Move move : moves)
    {
        if (game.board.at(move.to_square).color == opponent_color)
            valid_lifted_opponent.insert(move.to_square);

        // Add en passant piece to
        if (move.special_move)
        {
            int diff = move.to_square - move.from_square;
            // Right en passant both colors
            if (diff == 9 || diff == -7)
                valid_lifted_opponent.insert(move.to_square + 1);

            // Left en passant both colors
            if (diff == 7 || diff == -9)
                valid_lifted_opponent.insert(move.to_square - 1);
        }
    }

    for (int i = 0; i < game.board.size(); i++)
    {
        if (game.board.at(i).color == game.player_turn)
            valid_lifted.insert(i);
    }
}


std::pair<int, int> MakeMove::determineCastle(Move move, int lifted)
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

    return {-1, -1};
}


bool MakeMove::detectChangeTick(uint64_t tick_bit_board)
{
    struct BitChange
    {
        int index;
        bool placed;
    };

    uint64_t diff = current_bit_board ^ tick_bit_board;
    if (diff == 0)
        return false;

    std::vector<BitChange> changes;
    uint64_t temp_diff = diff;
    while (temp_diff)
    {
        int bit_diff = __builtin_ctzll(temp_diff);
        int index = 63 - bit_diff;
        bool placed_bool = (tick_bit_board >> bit_diff) & 1ULL;

        changes.push_back({index, placed_bool});
        temp_diff &= temp_diff - 1;
    }

    std::sort(changes.begin(), changes.end(),
        [](const BitChange& a, const BitChange& b)
        {
            auto priority = [](const BitChange& c)
            {
                // Highest priority: king lifts for castle
                if (!c.placed && (c.index == 4 || c.index == 60))
                    return 0;

                // Normal lifts
                if (!c.placed)
                    return 1;

                // Lowest pirority: placing
                return 2;
            };

            return priority(a) < priority(b);
        });

    for (const BitChange& change : changes)
    {
        int index = change.index;
        bool placed_bool = change.placed;

        // Lift piece if no pieces are lifted
        if (lifted == -1 && !placed_bool && valid_lifted.find(index) != valid_lifted.end())
        {
            lifted = index;
            continue;
        }

        // Lift opponent if a piece has been lifted
        if (lifted_opponent == -1 && !placed_bool && valid_lifted_opponent.find(index) != valid_lifted_opponent.end())
        {
            lifted_opponent = index;
            continue;
        }

        bool valid_placed_move = false;
        std::unordered_map<int, int> castle_moves_index; // m[lift] = placed
        for (Move move : moves)
        {
            //std::cout << "mf " << move.from_square << " mt " << move.to_square << " s " << move.special_move << std::endl;
            if (placed == -1 && placed_bool && move.to_square == index && move.from_square == lifted)
                valid_placed_move = true;
            else if (lifted == move.from_square && move.special_move)
            {
                std::pair<int, int> special_move = determineCastle(move, lifted);
                castle_moves_index[special_move.first] = special_move.second;
                std::cout << "s1: " << special_move.first << " s2: " << special_move.second << std::endl;
            }
        }

        if (placed_bool)
        {
            // Place piece at a valid position
            if (valid_placed_move && castle_moves_index[lifted_castle] != index)
                placed = index;

            // Place castle
            else if (castle_moves_index[lifted_castle] == index)
                placed_castle = index;

            // Place back lifted piece
            else if (lifted == index)
                lifted = -1;

            // Place back castle move
            else if (lifted_castle == index)
                lifted_castle = -1;

            // Place back illegally lifted
            else if (illegal_lifted.find(index) != illegal_lifted.end())
                illegal_lifted.erase(index);

            // Else placed illegally
            else
                illegal_lifted.insert(index);
        }
        else
        {
            // Lift castle move
            if (castle_moves_index.find(index) != castle_moves_index.end())
                lifted_castle = index;

            // Lift from castle placed
            else if (placed_castle == index)
                placed_castle = -1;

            // Lifted from illegal placed
            else if (illegal_placed.find(index) != illegal_placed.end())
                illegal_placed.erase(index);

            // If previous conditions aren't met, lift is illegal
            else
                illegal_lifted.insert(index);
        }
    }
    current_bit_board = tick_bit_board;
    if (lifted == -1 || lifted_opponent == -1)
        return true;

    // Remove opponent lifted if it doesn't match with current lifted
    bool check = false;
    for (Move move : moves)
    {
        if (move.from_square == lifted && move.to_square == lifted_opponent)
            check = true;
    }
    if (!check)
    {
        illegal_lifted.insert(lifted_opponent);
        lifted_opponent = -1;
    }
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
        if (moves.at(i).from_square == lifted && moves.at(i).to_square == placed &&
            current_bit_board == Chess::getGameBitBoard(moves.at(i).chess_game))
            return i;
    }
    return -1;
}


std::array<LedColor, 64> MakeMove::getBoardLights()
{
    std::array<LedColor, 64> lights;
    // Light past move first (can be overriden)
    if (past_move_from >= 0 && past_move_from < 64)
        lights.at(past_move_from) = past_move_color;
    if (past_move_to >= 0 && past_move_from < 64)
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

    // Move complete, with lingering clean up to do
    else if (lifted != -1 && placed != -1)
    {
        lights.at(lifted) = legal_moves_color;
        lights.at(placed) = legal_moves_color;
    }
    return lights;
}


std::string MakeMove::startOnline(const std::atomic<bool>& active)
{
    original_bit_board = Chess::getGameBitBoard(game);
    current_bit_board = Hardware::get().getBoardArr();
    if (original_bit_board != current_bit_board)
        return "error, original position not set";

    std::array<LedColor, 64> led_lights = getBoardLights();
    Hardware::get().setLed(led_lights);

    while (active.load() && timeout > 0)
    {
        uint64_t bit_board_tick = Hardware::get().getBoardArr();
        bool changes = detectChangeTick(bit_board_tick);
        if (changes)
        {
            int move_index = calculateMoveTick();
            if (move_index >= 0)
            {
                Hardware::get().clearLed();
                Move move_made = moves.at(move_index);
                return "ok, \"move_from\": " + std::to_string(move_made.from_square) +
                       ", \"move_to\": " + std::to_string(move_made.to_square) + "\"";
            }
            std::array<LedColor, 64> led_lights = getBoardLights();
            Hardware::get().setLed(led_lights);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timeout--;
    }
    Hardware::get().clearLed();
    return "error, timeout reached or end task called";
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
    original_bit_board = Chess::getGameBitBoard(game);
    current_bit_board = Hardware::get().getBoardArr();
    if (original_bit_board != current_bit_board)
        return Move{};

    std::array<LedColor, 64> led_lights = getBoardLights();
    Hardware::get().setLed(led_lights);

    while (active.load())
    {
        uint64_t bit_board_tick = Hardware::get().getBoardArr();
        bool changes = detectChangeTick(bit_board_tick);
        if (changes)
        {
            std::cout << "changes" << std::endl;
            std::cout << "bit_board_tick " << bit_board_tick << " lif" << lifted << " placed" << placed << std::endl;
            std::cout << "lifted " << lifted << std::endl;
            std::cout << "lifted_opponent " << lifted_opponent << std::endl;
            std::cout << "lifted_castle " << lifted_castle << std::endl;
            std::cout << "placed " << placed << std::endl;
            std::cout << "placed_castle " << placed_castle << std::endl;
            std::cout << "illegal_lifted: ";
            for (int val : illegal_lifted)
            {
                std::cout << val << " ";
            }
            std::cout << std::endl;

            std::cout << "illegal_placed: ";
            for (int val : illegal_placed)
            {
                std::cout << val << " ";
            }
            std::cout << std::endl;
            int move_index = calculateMoveTick();
            if (move_index >= 0)
            {
                Hardware::get().clearLed();
                return returnMove(active, move_index);
            }
            std::array<LedColor, 64> led_lights = getBoardLights();
            Hardware::get().setLed(led_lights);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Hardware::get().clearLed();
    return Move{};
}
