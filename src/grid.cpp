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
using namespace ui;

/*****************************************************************************/
ICell::ICell(uint x, uint y) noexcept
  : _x{ x }
  , _y{ y }
{}

/*****************************************************************************/
void
ICell::clear(void) noexcept
{
    if (EMPTY != _state)
        _changed = true;
    _state = EMPTY;
}

/*****************************************************************************/
void
ICell::clean(void) noexcept
{
    if (_state & PATH)
        _changed = true;
    _state &= ~PATH;
}

/*****************************************************************************/
void
ICell::setState(int st) noexcept
{
    if (st != _state) {
        _state = st;
        _changed = true;
    }
}

/*****************************************************************************/
void
ICell::addState(State st) noexcept
{
    if (!(_state & st)) {
        _state |= st;
        _changed = true;
    }
}

/*****************************************************************************/
void
ICell::remState(State st) noexcept
{
    if (_state & st) {
        _state &= ~st;
        _changed = true;
    }
}

/*****************************************************************************/
PathCell::PathCell(uint x, uint y, PathCell* parent)
  : ICell(x, y)
  , _parent{ parent }
{}

/*****************************************************************************/
void
PathCell::clean(void) noexcept
{
    ICell::clean();
    _G = _H = 0;
    _parent = nullptr;
}

/*****************************************************************************/
void
PathCell::clear(void) noexcept
{
    ICell::clear();
    _G = _H = 0;
    _parent = nullptr;
}

/*****************************************************************************/
uint
PathCell::getScore() const noexcept
{
    return _G + _H;
}

/*****************************************************************************/
Cell::Cell(uint x, uint y) noexcept
  : PathCell(x, y)
{}

/*****************************************************************************/
void
Cell::update(void) noexcept
{
    if (!_changed)
        return;

    setOutlineColor(Color::Black);
    setOutlineThickness(-1.f);

    if (EMPTY == _state) {
        setFillColor(Color(200, 200, 200, 250));
        return;
    }

    if (_state & (START_CELL | END_CELL))
        setFillColor(Color(234, 24, 24));
    else if (_state & WALL)
        setFillColor(Color::Black);
    else if (_state & PATH)
        setFillColor(Color(32, 32, 228));

    if (_state & SELECTED) {
        setOutlineColor(Color::Green);
        setOutlineThickness(-3.f);
    }

    _changed = false;
}

/*****************************************************************************/
Grid::Grid() noexcept
  : Graph<ui::Cell>()
{}

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
