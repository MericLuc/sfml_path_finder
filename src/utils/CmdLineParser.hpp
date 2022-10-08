/**
 * @file CmdLineParser.hpp
 * @brief Command-line parsing class
 * @author lhm
 */

#ifndef SRC_UTILS_CMDLINEPARSER_HPP
#define SRC_UTILS_CMDLINEPARSER_HPP

#include <string>
#include <vector>

/*****************************************************************************/
/*!
 * \brief The CmdLineParser class offers basic (dummy) command line parsing
 * operations
 */
class CmdLineParser
{
public:
    CmdLineParser(const int& argc, char** argv) noexcept;

    std::string_view getCmdOption(const std::string& opt) const noexcept;
    std::string_view getCmd(const size_t& idx) const noexcept;

    bool cmdOptionExists(const std::string& opt) const noexcept;

private:
    std::vector<std::string> _tokens{};
};

#endif // SRC_UTILS_CMDLINEPARSER_HPP
