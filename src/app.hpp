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

// Project's headers
#include <algo/astar.hpp>
#include <env/graph.hpp>
#include <graphics/grid.hpp>

namespace sf {
class RenderWindow;
class RectangleShape;
struct KeyEvent;
}

namespace JSON {
class Object;
}

template<typename T>
using UPTR = std::unique_ptr<T>;

/*****************************************************************************/
class App
{
public:
    typedef enum
    {
        CLEAN,
        ANALYZE,
        EXIT,
        RELOAD
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
    void _reload(void) noexcept;

    bool _initGraphics(const JSON::Object&) noexcept;
    bool _initBindings(const JSON::Object&) noexcept;
    bool _initAnalyzer(const JSON::Object&) noexcept;
    bool _initGrid(const JSON::Object& conf) noexcept;

protected:
    UPTR<sf::RenderWindow>               _window;
    UPTR<env::Graph<env::AStarCell>>     _graph;
    UPTR<graphics::Grid<env::AStarCell>> _grid;
    UPTR<astar::Impl<env::AStarCell>>    _analyzer;

    env::AStarCell* _cell_start{ nullptr };
    env::AStarCell* _cell_end{ nullptr };
    env::AStarCell* _cell_cur{ nullptr };

    std::string _what;
    std::string _conf_fileName;

    const std::map<ACTION, ActionFunction> _actionsBoundings;

private:
    App(const App&) noexcept = delete;
    App(App&&) noexcept = delete;

    App() noexcept;
    virtual ~App() noexcept;
};

#endif // SRC_APP_HPP
