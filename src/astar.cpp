/**
 * @file astar.cpp
 * @brief Implementation of \a astar.hpp
 * @author lhm
 */

// Standard headers
#include <algorithm>
#include <math.h>

// Project's headers
#include <astar.hpp>

namespace astar {

const std::vector<coord> DIRS{ { 0, 1 },   { 1, 0 }, { 0, -1 }, { -1, 0 },
                               { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };

/*****************************************************************************/
Impl::Impl() noexcept
  : _heuristic{ std::bind(&Heuristic::manhattan, std::placeholders::_1, std::placeholders::_2) }
  , _dirs{ 8 }
{}

/*****************************************************************************/
bool
Impl::configure(const JSON::Object&) noexcept
{
    // TODO
    return true;
}

/*****************************************************************************/
bool
Impl::run(ui::Grid* world, PathCell* start, PathCell* end) noexcept
{
    _world = world;
    if (nullptr == _world || nullptr == start || nullptr == end)
        return false;

    PathCell* cur{ nullptr };
    CellsList open;
    CellsSet  closed;

    open.push_back(start);

    while (!std::empty(open)) {
        auto it{ std::max_element(std::begin(open), std::end(open), [](PathCell* a, PathCell* b) {
            return a->getScore() >= b->getScore();
        }) };
        cur = *it;

        if (end == cur)
            break;

        closed.insert(cur);
        open.erase(it);

        for (uint i{ 0 }; i < _dirs; ++i) {
            auto neigh{ _world->cell(cur->x() + DIRS[i].x, cur->y() + DIRS[i].y) };
            if (!_eligible(neigh) || closed.count(neigh))
                continue;

            uint totalCost{ cur->_G + ((i < 4) ? 10 : 14) };

            if (std::end(open) == std::find(std::begin(open), std::end(open), neigh)) {
                neigh->_parent = cur;
                neigh->_G = totalCost;
                neigh->_H = _heuristic(neigh, end);
                open.push_back(neigh);
            } else if (totalCost < neigh->_G) {
                neigh->_parent = cur;
                neigh->_G = totalCost;
            }
        }
    }

    while (nullptr != cur) {
        cur->addState(ICell::PATH);
        cur = cur->_parent;
    }

    return true;
}

/*****************************************************************************/
bool
Impl::_eligible(PathCell* cell) noexcept
{
    if (nullptr == cell)
        return false;

    return !cell->hasState(ICell::WALL);
}

/*****************************************************************************/
coord
Heuristic::getDelta(PathCell* s, PathCell* d) noexcept
{
    return { abs(static_cast<int>(s->x()) - static_cast<int>(d->x())),
             abs(static_cast<int>(s->y()) - static_cast<int>(d->y())) };
}

/*****************************************************************************/
uint
Heuristic::manhattan(PathCell* s, PathCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return static_cast<uint>(10 * (delta.x + delta.y));
}

/*****************************************************************************/
uint
Heuristic::euclidean(PathCell* s, PathCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

/*****************************************************************************/
uint
astar::Heuristic::octagonal(PathCell* s, PathCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}

}
