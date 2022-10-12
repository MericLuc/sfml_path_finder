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
bool
ICell::clear(void) noexcept
{
    if (EMPTY != _state) {
        _state = EMPTY;
        return true;
    }
    return false;
}

/*****************************************************************************/
bool
ICell::clean(void) noexcept
{
    if (_state & PATH) {
        _state &= ~PATH;
        return true;
    }
    return false;
}

/*****************************************************************************/
bool
ICell::setState(int st) noexcept
{
    if (st != _state) {
        _state = st;
        return true;
    }
    return false;
}

/*****************************************************************************/
bool
ICell::addState(State st) noexcept
{
    if (!(_state & st)) {
        _state |= st;
        return true;
    }
    return false;
}

/*****************************************************************************/
bool
ICell::remState(State st) noexcept
{
    if (_state & st) {
        _state &= ~st;
        return true;
    }
    return false;
}

/*****************************************************************************/
PathCell::PathCell(uint x, uint y, PathCell* parent)
  : ICell(x, y)
  , _parent{ parent }
{}

/*****************************************************************************/
bool
PathCell::clean(void) noexcept
{
    _G = _H = 0;
    _parent = nullptr;

    return ICell::clean();
}

/*****************************************************************************/
bool
PathCell::clear(void) noexcept
{
    _G = _H = 0;
    _parent = nullptr;

    return ICell::clear();
}

/*****************************************************************************/
uint
PathCell::getScore() const noexcept
{
    return _G + _H;
}

/*****************************************************************************/
Grid::Grid() noexcept
  : Graph<PathCell>()
  , _vertexes{ _width * _height * 4, Vertex() }
{}

/*****************************************************************************/
void
Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    static double cell_width{ 0 };
    static double cell_height{ 0 };

    bool       updt{ false };
    const auto new_width{ target.getSize().x / static_cast<double>(_width) };
    const auto new_height{ target.getSize().y / static_cast<double>(_height) };

    if (new_width != cell_width || new_height != cell_height) {
        cell_width = new_width;
        cell_height = new_height;
        updt = true;
    }

    for (uint i{ 0 }; i < _width; ++i)
        for (uint j{ 0 }; j < _height; ++j) {
            if (updt)
                updateCell(i,
                           j,
                           Vector2f(cell_width * i, cell_height * j),
                           Vector2f(cell_width, cell_height));

            updateCellStyle(i, j);
        }

    target.draw(_vertexes.data(), std::size(_vertexes), sf::Quads, states);
}

/*****************************************************************************/
void
Grid::updateCellStyle(uint i, uint j) const noexcept
{
    auto      st{ _grid[i + j * _width].getState() };
    auto      idx{ (i + j * _width) * 4 };
    sf::Color color{ 200, 200, 200, 250 };

    if (st & (ICell::START_CELL | ICell::END_CELL)) {
        color = Color(220, 20, 20, 250);
    } else if (st & ICell::PATH) {
        color = Color(32, 32, 228);
    } else if (st & ICell::WALL) {
        color = Color::Black;
    }

    for (auto it{ 0 }; it < 4; ++it)
        _vertexes[idx + it].color = color;
}

/*****************************************************************************/
void
Grid::updateCell(uint i, uint j, Vector2f pos, Vector2f size) const noexcept
{
    auto idx{ (i + j * _width) * 4 };

    _vertexes[idx].position = { pos.x, pos.y };
    _vertexes[idx + 1].position = { pos.x + size.x, pos.y };
    _vertexes[idx + 2].position = { pos.x + size.x, pos.y + size.y };
    _vertexes[idx + 3].position = { pos.x, pos.y + size.y };
}
