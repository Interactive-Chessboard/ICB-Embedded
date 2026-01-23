// extarct_set_board.cpp
#include "extract_set_board.hpp"


uint64_t to_uint64(const std::string &request)
{
    try
    {
        std::string value = extractValue(request, "board");

        if (!value.empty() && value[0] == '-')
            throw std::runtime_error("Error, number must be a valid uint64");

        return std::stoull(value);
    }
    catch (...)
    {
        throw std::runtime_error("Error, number must be a valid uint64");
    }
}


ExtractSetBoard extractSetBoard(const std::string& request)
{
    ExtractSetBoard set_board;
    set_board.color = getLedColor(extractValue(request, "color"));
    set_board.board = to_uint64(request);
    set_board.timeout = extractTimeOut(request);
    return set_board;
}
