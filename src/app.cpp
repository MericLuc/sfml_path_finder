/**
 * @file app.cpp
 * @brief Implementation of \a app.hpp
 * @author lhm
 */

// Standard headers

// Project headers
#include <app.hpp>
#include <grid.hpp>

// External libs
#include <SFML/Graphics.hpp>

using namespace sf;

constexpr int WINDOW_DEFAULT_HEIGHT{ 640 };
constexpr int WINDOW_DEFAULT_WIDTH{ 640 };

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
                    nullptr != _cell_cur)
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
                // Process key events
                // (clean, start algorithm...)
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
