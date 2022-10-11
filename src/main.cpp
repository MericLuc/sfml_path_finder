/**
 * @file main.cpp
 * @brief Main
 * @author lhm
 */

// Standard headers
#include <iostream>
#include <stdlib.h>

// Project headers
#include <app.hpp>
#include <utils/CmdLineParser.hpp>

/*****************************************************************************/
static void
help(void)
{
    std::cout << PROG_NAME << " : Path finder toy (SFML discover)\n\n"
              << "Usage: " << PROG_NAME << " [-opt val]\n"
              << "Options: \n\t" << CMDLINE_HELP << " : Display the help\n"
              << "\n\t" << CMDLINE_CONF << " filename : Set configuration file\n\n";
}

static const std::string default_conf{ "/etc/" PROG_NAME };

/*****************************************************************************/
int
main(int argc, char* argv[])
{
    auto& app{ App::get() };
    auto  parser{ std::make_unique<CmdLineParser>(argc, argv) };

    if (parser->cmdOptionExists(CMDLINE_HELP)) {
        help();
        return EXIT_SUCCESS;
    }

    if (!app.configure(parser->cmdOptionExists(CMDLINE_CONF) ? parser->getCmdOption(CMDLINE_CONF)
                                                             : default_conf)) {
        std::cerr << app.what() << '\n';
        return EXIT_FAILURE;
    }

    while (app) {
        app.update();
        app.render();
    }

    return EXIT_SUCCESS;
}
