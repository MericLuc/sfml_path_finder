/**
 * @file main.cpp
 * @brief Main
 * @author lhm
 */

#include <app.hpp>
#include <stdlib.h>

#include <SFML/Graphics.hpp>

/*****************************************************************************/
int
main()
{
    auto& app{ App::get() };

    // TODO - Add frameRate
    while (app) {
        app.update();
        app.render();
    }

    return EXIT_SUCCESS;
}
