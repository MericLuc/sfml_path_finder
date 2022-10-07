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
App::_handleEvts() noexcept
{
    sf::Event event;
    while (_window->pollEvent(event)) {
        switch (event.type) {
            case Event::Resized:
                _window->setView(View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
            case Event::Closed:
                _window->close();
                break;
            case Event::MouseMoved:
                // Update
                break;
            default:
                break;
        }
    }
}

/*****************************************************************************/
void
App::_handleMouse() noexcept
{
    static Cell* selected_cell{ nullptr };

    if (nullptr != selected_cell)
        selected_cell->setState(Cell::EMPTY);

    auto mousePos{ Mouse::getPosition(*_window) };

    if (selected_cell = _grid->cell(
          mousePos.x / (static_cast<double>(_window->getSize().x) / _grid->getWidth()),
          mousePos.y / (static_cast<double>(_window->getSize().y) / _grid->getHeight()));
        nullptr != selected_cell)
        selected_cell->setState(Cell::SELECTED);
}

/*****************************************************************************/
void
App::update(void) noexcept
{
    _handleEvts();
    _handleMouse();
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
