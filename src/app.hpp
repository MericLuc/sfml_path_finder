/**
 * @file app.hpp
 * @brief App isa singleton containing the application logic
 * @author lhm
 */

#ifndef SRC_APP_HPP
#define SRC_APP_HPP

// Standard headers
#include <memory>

namespace sf {
class RenderWindow;
class RectangleShape;
}

class Grid;
class Cell;

/*****************************************************************************/
class App
{
public:
    static App& get(void) noexcept;

    operator bool() noexcept;

    virtual void update(void) noexcept;
    virtual void render(void) noexcept;

protected:
    std::unique_ptr<sf::RenderWindow> _window;
    std::unique_ptr<Grid>             _grid;

    Cell* _cell_start{ nullptr };
    Cell* _cell_end{ nullptr };
    Cell* _cell_cur{ nullptr };

private:
    App(const App&) noexcept = delete;
    App(App&&) noexcept = delete;

    App() noexcept;
    virtual ~App() noexcept;
};

#endif // SRC_APP_HPP
