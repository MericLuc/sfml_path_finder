/**
 * @file CmdLineParser.cpp
 * @brief Implementation of \a CmdLineParser.hpp
 * @author lhm
 */

#include <utils/CmdLineParser.hpp>

#include <algorithm>

static constexpr std::string_view empty{};

/*****************************************************************************/
CmdLineParser::CmdLineParser(const int& argc, char** argv) noexcept
{
    for (int i = 1; i < argc; ++i)
        _tokens.emplace_back(argv[i]);
}

/*****************************************************************************/
std::string_view
CmdLineParser::getCmdOption(const std::string& opt) const noexcept
{
    if (auto it = std::find(std::begin(_tokens), std::end(_tokens), opt);
        it != std::end(_tokens) && std::end(_tokens) != ++it) {
        return *it;
    }

    return empty;
}

/*****************************************************************************/
std::string_view
CmdLineParser::getCmd(const size_t& idx) const noexcept
{
    return (std::size(_tokens) < idx) ? empty : _tokens.at(idx - 1);
}

/*****************************************************************************/
bool
CmdLineParser::cmdOptionExists(const std::string& opt) const noexcept
{
    return std::end(_tokens) != std::find(std::begin(_tokens), std::end(_tokens), opt);
}
