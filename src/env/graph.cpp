/**
 * @file app.cpp
 * @brief Implementation of \a app.hpp
 * @author lhm
 */

// Standard headers

// Project headers
#include "graph.hpp"

// External libs
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace env;

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
AStarCell::AStarCell(uint x, uint y, AStarCell* parent)
  : ICell(x, y)
  , _parent{ parent }
{}

/*****************************************************************************/
bool
AStarCell::clean(void) noexcept
{
    _G = _H = 0;
    _parent = nullptr;

    return ICell::clean();
}

/*****************************************************************************/
bool
AStarCell::clear(void) noexcept
{
    _G = _H = 0;
    _parent = nullptr;

    return ICell::clear();
}

/*****************************************************************************/
uint
AStarCell::getScore() const noexcept
{
    return _G + _H;
}
