/**
 * @file app.cpp
 * @brief Implementation of \a app.hpp
 * @author lhm
 */

// Standard headers
#include <filesystem>

// Project headers
#include <app.hpp>
#include <astar.hpp>
#include <grid.hpp>
#include <utils/Json.hpp>

// External libs
#include <JSON.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace ui;
namespace fs = std::filesystem;

constexpr int WINDOW_DEFAULT_HEIGHT{ 640 };
constexpr int WINDOW_DEFAULT_WIDTH{ 640 };

std::map<sf::Keyboard::Key, App::ACTION> _bindings;
bool                                     need_cleaning{ false };

/*****************************************************************************/
App&
App::get(void) noexcept
{
    static App instance;
    return instance;
}

/*****************************************************************************/
App::operator bool() noexcept
{
    return _window->isOpen();
}

/*****************************************************************************/
bool
App::configure(const std::string_view& file) noexcept
{
    _conf_fileName = file;
    fs::path conf_path{ _conf_fileName };
    if (std::empty(_conf_fileName)) {
        _what = "Missing or empty file path";
        return false;
    }

    if (!fs::exists(conf_path)) {
        _what = "Input file does not exist";
        return false;
    }

    JSON::Object obj{ JSON::Object::fromFile(conf_path) };
    if (!json_test_struct(
          obj,
          { { "bindings", 'o' }, { "graphics", 'o' }, { "grid", 'o' }, { "analyzer", 'o' } })) {
        _what = "Wrong format for configuration file";
        return false;
    }

    if (!_initGraphics(obj["graphics"]))
        return false;

    if (!_initBindings(obj["bindings"]))
        return false;

    if (!_initAnalyzer(obj["analyzer"]))
        return false;

    if (!_initGrid(obj["grid"]))
        return false;

    return true;
}

/*****************************************************************************/
void
App::update(void) noexcept
{
    static bool locked_click{ false };

    sf::Event event;
    while (_window->pollEvent(event)) {
        switch (event.type) {
            case Event::Resized: {
                _window->setView(View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            } break;
            case Event::Closed: {
                _window->close();
            } break;
            case Event::MouseEntered:
            case Event::MouseMoved: {
                if (nullptr != _cell_cur)
                    _cell_cur->remState(Cell::SELECTED);

                auto mousePos{ Mouse::getPosition(*_window) };

                if (_cell_cur = _grid->cell(
                      mousePos.x / (static_cast<double>(_window->getSize().x) / _grid->getWidth()),
                      mousePos.y /
                        (static_cast<double>(_window->getSize().y) / _grid->getHeight()));
                    nullptr == _cell_cur)
                    break;

                _cell_cur->addState(Cell::SELECTED);

                if (locked_click &&
                    !(_cell_cur->getState() & (Cell::START_CELL | Cell::END_CELL))) {
                    _cell_cur->addState(Cell::WALL);
                }

            } break;
            case Event::MouseLeft: {
                if (nullptr != _cell_cur) {
                    _cell_cur->remState(Cell::SELECTED);
                    _cell_cur = nullptr;
                }
            } break;
            case Event::MouseButtonPressed: {
                if (need_cleaning) {
                    _grid->clean();
                    need_cleaning = false;
                }
                switch (event.mouseButton.button) {
                    case Mouse::Button::Left: {
                        locked_click = true;
                    } break;
                    default:
                        break;
                }
            } break;
            case Event::MouseButtonReleased: {
                if (nullptr == _cell_cur)
                    break;
                switch (event.mouseButton.button) {
                    case Mouse::Button::Left: {
                        locked_click = false;
                        if (!_cell_cur->hasState(Cell::START_CELL | Cell::END_CELL))
                            _cell_cur->addState(Cell::WALL);

                    } break;
                    case Mouse::Button::Right: {
                        if (_cell_cur->hasState(Cell::WALL))
                            break;

                        if (_cell_cur == _cell_start) {
                            _cell_cur->remState(Cell::START_CELL);
                            _cell_start = nullptr;
                        } else if (_cell_cur == _cell_end) {
                            _cell_cur->remState(Cell::END_CELL);
                            _cell_end = nullptr;
                        } else if (nullptr == _cell_start) {
                            _cell_start = _cell_cur;
                            _cell_start->addState(Cell::START_CELL);
                        } else if (nullptr == _cell_end) {
                            _cell_end = _cell_cur;
                            _cell_end->addState(Cell::END_CELL);
                        } else {
                            _cell_end->remState(Cell::END_CELL);
                            _cell_end = _cell_cur;
                            _cell_end->addState(Cell::END_CELL);
                        }
                    } break;
                    default:
                        break;
                }
            } break;
            case Event::KeyReleased: {
                if (auto it{ _bindings.find(event.key.code) }; std::end(_bindings) != it) {
                    _actionsBoundings.at(it->second)();
                }
            } break;
            default:
                break;
        }
    }
}

/*****************************************************************************/
void
App::render(void) noexcept
{
    _window->clear();
    _window->draw(*_grid);
    _window->display();
}

/*****************************************************************************/
App::App() noexcept
  : _window{ std::make_unique<RenderWindow>(VideoMode(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT),
                                            PROG_NAME,
                                            Style::Default) }
  , _grid{ std::make_unique<Grid>() }
  , _analyzer{ std::make_unique<astar::Impl>() }
  , _actionsBoundings{ { App::CLEAN, [this]() { _clear(); } },
                       { App::ANALYZE, [this]() { _analyze(); } },
                       { App::EXIT, [this]() { _stop(); } },
                       { App::RELOAD, [this]() { _reload(); } } }
{
    View view;
    view.setSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    view.setCenter(_window->getSize().x / 2.f, _window->getSize().y / 2.f);
    _window->setView(view);
}

/*****************************************************************************/
App::~App() noexcept
{
    // TODO
}

/*****************************************************************************/
void
App::_clear(void) noexcept
{
    need_cleaning = false;
    _grid->clear();
    _cell_start = nullptr;
    _cell_end = nullptr;
}

/*****************************************************************************/
void
App::_analyze(void) noexcept
{
    if (nullptr == _cell_start || nullptr == _cell_end)
        return;
    _grid->clean();
    _analyzer->run(_grid.get(), _cell_start, _cell_end);
    need_cleaning = true;
}

/*****************************************************************************/
void
App::_stop(void) noexcept
{
    _window->close();
}

/*****************************************************************************/
void
App::_reload(void) noexcept
{
    configure(_conf_fileName);
}

/*****************************************************************************/
bool
App::_initGraphics(const JSON::Object& conf) noexcept
{
    if (!json_test_struct(conf, { { "height", 'i' }, { "width", 'i' }, { "frame-rate", 'i' } })) {
        _what = "Cannot initialize 'graphics' : wrong format";
        return false;
    }

    _window->setVerticalSyncEnabled(false);
    _window->setSize(sf::Vector2u(conf["width"].asInt(), conf["height"].asInt()));
    _window->setFramerateLimit(conf["frame-rate"].asInt());

    return true;
}

/*****************************************************************************/
bool
App::_initBindings(const JSON::Object& conf) noexcept
{
    _bindings.clear();

    static const std::map<std::string, sf::Keyboard::Key> _cvt{
        { "Unknown", sf::Keyboard::Key::Unknown },
        { "A", sf::Keyboard::Key::A },
        { "B", sf::Keyboard::Key::B },
        { "C", sf::Keyboard::Key::C },
        { "D", sf::Keyboard::Key::D },
        { "E", sf::Keyboard::Key::E },
        { "F", sf::Keyboard::Key::F },
        { "G", sf::Keyboard::Key::G },
        { "H", sf::Keyboard::Key::H },
        { "I", sf::Keyboard::Key::I },
        { "J", sf::Keyboard::Key::J },
        { "K", sf::Keyboard::Key::K },
        { "L", sf::Keyboard::Key::L },
        { "M", sf::Keyboard::Key::M },
        { "N", sf::Keyboard::Key::N },
        { "O", sf::Keyboard::Key::O },
        { "P", sf::Keyboard::Key::P },
        { "Q", sf::Keyboard::Key::Q },
        { "R", sf::Keyboard::Key::R },
        { "S", sf::Keyboard::Key::S },
        { "T", sf::Keyboard::Key::T },
        { "U", sf::Keyboard::Key::U },
        { "V", sf::Keyboard::Key::V },
        { "W", sf::Keyboard::Key::W },
        { "X", sf::Keyboard::Key::X },
        { "Y", sf::Keyboard::Key::Y },
        { "Z", sf::Keyboard::Key::Z },
        { "Num0", sf::Keyboard::Key::Num0 },
        { "Num1", sf::Keyboard::Key::Num1 },
        { "Num2", sf::Keyboard::Key::Num2 },
        { "Num3", sf::Keyboard::Key::Num3 },
        { "Num4", sf::Keyboard::Key::Num4 },
        { "Num5", sf::Keyboard::Key::Num5 },
        { "Num6", sf::Keyboard::Key::Num6 },
        { "Num7", sf::Keyboard::Key::Num7 },
        { "Num8", sf::Keyboard::Key::Num8 },
        { "Num9", sf::Keyboard::Key::Num9 },
        { "Escape", sf::Keyboard::Key::Escape },
        { "LControl", sf::Keyboard::Key::LControl },
        { "LShift", sf::Keyboard::Key::LShift },
        { "LAlt", sf::Keyboard::Key::LAlt },
        { "LSystem", sf::Keyboard::Key::LSystem },
        { "RControl", sf::Keyboard::Key::RControl },
        { "RShift", sf::Keyboard::Key::RShift },
        { "RAlt", sf::Keyboard::Key::RAlt },
        { "RSystem", sf::Keyboard::Key::RSystem },
        { "Menu", sf::Keyboard::Key::Menu },
        { "LBracket", sf::Keyboard::Key::LBracket },
        { "RBracket", sf::Keyboard::Key::RBracket },
        { "Semicolon", sf::Keyboard::Key::Semicolon },
        { "Comma", sf::Keyboard::Key::Comma },
        { "Period", sf::Keyboard::Key::Period },
        { "Quote", sf::Keyboard::Key::Quote },
        { "Slash", sf::Keyboard::Key::Slash },
        { "Backslash", sf::Keyboard::Key::Backslash },
        { "Tilde", sf::Keyboard::Key::Tilde },
        { "Equal", sf::Keyboard::Key::Equal },
        { "Hyphen", sf::Keyboard::Key::Hyphen },
        { "Space", sf::Keyboard::Key::Space },
        { "Enter", sf::Keyboard::Key::Enter },
        { "Backspace", sf::Keyboard::Key::Backspace },
        { "Tab", sf::Keyboard::Key::Tab },
        { "PageUp", sf::Keyboard::Key::PageUp },
        { "PageDown", sf::Keyboard::Key::PageDown },
        { "End", sf::Keyboard::Key::End },
        { "Home", sf::Keyboard::Key::Home },
        { "Insert", sf::Keyboard::Key::Insert },
        { "Delete", sf::Keyboard::Key::Delete },
        { "Add", sf::Keyboard::Key::Add },
        { "Subtract", sf::Keyboard::Key::Subtract },
        { "Multiply", sf::Keyboard::Key::Multiply },
        { "Divide", sf::Keyboard::Key::Divide },
        { "Left", sf::Keyboard::Key::Left },
        { "Right", sf::Keyboard::Key::Right },
        { "Up", sf::Keyboard::Key::Up },
        { "Down", sf::Keyboard::Key::Down },
        { "Numpad0", sf::Keyboard::Key::Numpad0 },
        { "Numpad1", sf::Keyboard::Key::Numpad1 },
        { "Numpad2", sf::Keyboard::Key::Numpad2 },
        { "Numpad3", sf::Keyboard::Key::Numpad3 },
        { "Numpad4", sf::Keyboard::Key::Numpad4 },
        { "Numpad5", sf::Keyboard::Key::Numpad5 },
        { "Numpad6", sf::Keyboard::Key::Numpad6 },
        { "Numpad7", sf::Keyboard::Key::Numpad7 },
        { "Numpad8", sf::Keyboard::Key::Numpad8 },
        { "Numpad9", sf::Keyboard::Key::Numpad9 },
        { "F1", sf::Keyboard::Key::F1 },
        { "F2", sf::Keyboard::Key::F2 },
        { "F3", sf::Keyboard::Key::F3 },
        { "F4", sf::Keyboard::Key::F4 },
        { "F5", sf::Keyboard::Key::F5 },
        { "F6", sf::Keyboard::Key::F6 },
        { "F7", sf::Keyboard::Key::F7 },
        { "F8", sf::Keyboard::Key::F8 },
        { "F9", sf::Keyboard::Key::F9 },
        { "F10", sf::Keyboard::Key::F10 },
        { "F11", sf::Keyboard::Key::F11 },
        { "F12", sf::Keyboard::Key::F12 },
        { "F13", sf::Keyboard::Key::F13 },
        { "F14", sf::Keyboard::Key::F14 },
        { "F15", sf::Keyboard::Key::F15 },
        { "Pause", sf::Keyboard::Key::Pause }
    };

    if (!json_test_struct(
          conf, { { "clear", 's' }, { "analyze", 's' }, { "exit", 's' }, { "reload", 's' } })) {
        _what = "Cannot initialize 'bindings' : wrong format";
        return false;
    }

    if (auto it{ _cvt.find(conf["clear"].asString()) }; std::end(_cvt) != it)
        _bindings[it->second] = CLEAN;

    if (auto it{ _cvt.find(conf["analyze"].asString()) }; std::end(_cvt) != it)
        _bindings[it->second] = ANALYZE;

    if (auto it{ _cvt.find(conf["exit"].asString()) }; std::end(_cvt) != it)
        _bindings[it->second] = EXIT;

    if (auto it{ _cvt.find(conf["reload"].asString()) }; std::end(_cvt) != it)
        _bindings[it->second] = RELOAD;

    return true;
}

/*****************************************************************************/
bool
App::_initAnalyzer(const JSON::Object& conf) noexcept
{
    auto ret{ _analyzer->configure(conf) };

    if (!ret)
        _what = "Cannot initialize 'analyzer' : wrong format";

    return ret;
}

/*****************************************************************************/
bool
App::_initGrid(const JSON::Object& conf) noexcept
{
    size_t rows = conf["rows"].asInt();
    size_t cols = conf["cols"].asInt();

    if (0 == rows || 0 == cols) {
        _what = "Cannot initialize 'grid' : Invalid values for 'dimensions'";
        return false;
    }

    if (_grid->getHeight() != rows || _grid->getWidth() != cols) {
        _cell_cur = nullptr;
        _cell_start = nullptr;
        _cell_end = nullptr;
        _grid->resize(cols, rows);
    }

    return true;
}
