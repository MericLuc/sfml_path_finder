/**
 * @file app.hpp
 * @brief App is a singleton containing the application logic
 * @author lhm
 */

#ifndef SRC_APP_HPP
#define SRC_APP_HPP

// Standard headers
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>

// Project's headers
#include <grid.hpp>

namespace sf {
class RenderWindow;
class RectangleShape;
struct KeyEvent;
}

namespace JSON {
class Object;
}

namespace astar {
class Impl;
}

/*****************************************************************************/
class App
{
public:
    typedef enum
    {
        CLEAN,
        ANALYZE,
        STOP
    } ACTION;
    using ActionFunction = std::function<void(void)>;

public:
    static App& get(void) noexcept;

    operator bool() noexcept;

    [[maybe_unused]] virtual bool configure(const std::string_view& file) noexcept;
    virtual void                  update(void) noexcept;
    virtual void                  render(void) noexcept;

    std::string what(void) const noexcept { return _what; }

protected:
    void _clear(void) noexcept;
    void _analyze(void) noexcept;
    void _stop(void) noexcept;

    bool _initGraphics(const JSON::Object&) noexcept;
    bool _initBindings(const JSON::Object&) noexcept;
    bool _initAnalyzer(const JSON::Object&) noexcept;

protected:
    std::unique_ptr<sf::RenderWindow> _window;
    std::unique_ptr<ui::Grid>         _grid;
    std::unique_ptr<astar::Impl>      _analyzer;

    ui::Cell* _cell_start{ nullptr };
    ui::Cell* _cell_end{ nullptr };
    ui::Cell* _cell_cur{ nullptr };

    std::string _what;

    const std::map<App::ACTION, App::ActionFunction> _actionsBoundings;

private:
    App(const App&) noexcept = delete;
    App(App&&) noexcept = delete;

    App() noexcept;
    virtual ~App() noexcept;
};

#endif // SRC_APP_HPP
