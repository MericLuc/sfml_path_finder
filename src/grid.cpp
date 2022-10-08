/**
 * @file app.cpp
 * @brief Implementation of \a app.hpp
 * @author lhm
 */

// Standard headers

// Project headers
#include <grid.hpp>

// External libs
#include <SFML/Graphics.hpp>

using namespace sf;

/*****************************************************************************/
void
Cell::update(void) noexcept
{
    setOutlineColor(Color::Black);
    setOutlineThickness(-1.f);

    if (EMPTY == _state) {
        setFillColor(Color(200, 200, 200, 250));
        return;
    }

    if (_state & (START_CELL | END_CELL))
        setFillColor(Color(234, 24, 24));

    if (_state & WALL)
        setFillColor(Color::Black);

    if (_state & PATH)
        setFillColor(Color(32, 32, 228));

    if (_state & SELECTED) {
        setOutlineColor(Color::Green);
        setOutlineThickness(-3.f);
    }
}

/*****************************************************************************/
Grid::Grid(size_t width, size_t height) noexcept
  : _width{ width }
  , _height{ height }
  , _grid{ _height * _width, Cell() }
{}

/*****************************************************************************/
void
Grid::clean(void) noexcept
{
    for (auto& cell : _grid)
        cell.setState(Cell::EMPTY);
}

/*****************************************************************************/
void
Grid::resize(size_t width, size_t height) noexcept
{
    _width = width;
    _height = height;
    _grid = std::vector<Cell>(_width * _height, Cell());
}

/*****************************************************************************/
void
Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const auto cell_width{ target.getSize().x / static_cast<double>(_width) };
    const auto cell_height{ target.getSize().y / static_cast<double>(_height) };

    for (size_t i{ 0 }; i < _width; ++i)
        for (size_t j{ 0 }; j < _height; ++j) {
            auto& cell{ _grid[i + j * _width] };

            cell.update();
            cell.setSize(Vector2f(cell_width, cell_height));
            cell.setPosition(Vector2f(cell_width * i, cell_height * j));

            target.draw(cell, states);
        }
}
