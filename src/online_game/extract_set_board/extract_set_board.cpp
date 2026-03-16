// extarct_set_board.cpp
#include "extract_set_board.hpp"


ExtractSetBoard extractSetBoard(const std::string& request)
{
    ExtractSetBoard set_board;
    set_board.game = getChessGameBoard(request);

    std::string past_move_str = extractValue(request, "past_move");
    set_board.past_move_color = getLedColor(extractValue(past_move_str, "color"));
    set_board.past_move_from = stoi(extractValue(past_move_str, "from"));
    set_board.past_move_to = stoi(extractValue(past_move_str, "to"));

    set_board.timeout = extractTimeOut(request);
    return set_board;
}
