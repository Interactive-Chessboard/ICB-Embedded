// extarct_set_board.cpp
#include "extract_set_board.hpp"


uint64_t to_uint64(const std::string &request)
{
    try
    {
        return std::stoull(extract_value(request, "board"));
    }
    catch (...)
    {
        throw std::runtime_error("Error, number must be a valid uint64");
    }
}


ExtractSetBoard extractSetBoard(const std::string& request)
{
    ExtractSetBoard set_board;
    set_board.color = getColor(extract_value(request, "color"));
    set_board.board = to_uint64(request);
    set_board.timeout = extractTimeOut(request) * 100;
    return set_board;
}
