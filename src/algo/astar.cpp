/**
 * @file astar.cpp
 * @brief Implementation of \a astar.hpp
 * @author lhm
 */

// Standard headers
#include <algorithm>
#include <math.h>

// Project's headers
#include "astar.hpp"
#include <utils/Json.hpp>

// External headers
#include <JSON.hpp>

using namespace env;

namespace astar {

using CellsList = std::list<ICell*>;
using CellsSet = std::set<ICell*>;

template<typename T>
const std::vector<std::pair<int, int>> Impl<T>::DIRS{ { 0, 1 },   { 1, 0 }, { 0, -1 }, { -1, 0 },
                                                      { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };

/*****************************************************************************/
template<typename T>
Impl<T>::Impl() noexcept
  : _heuristic{ std::bind(&Heuristic::manhattan, std::placeholders::_1, std::placeholders::_2) }
  , _dirs{ 8 }
{}

/*****************************************************************************/
template<typename T>
bool
Impl<T>::configure(const JSON::Object& conf) noexcept
{
    if (!json_test_struct(conf, { { "heuristic", 's' }, { "allow-diagonals", 'b' } }))
        goto error;

    {
        std::string heuristic_name{ conf["heuristic"].asString() };
        if (!heuristic_name.compare("euclidean")) {
            _heuristic =
              std::bind(&Heuristic::euclidean, std::placeholders::_1, std::placeholders::_2);
        } else if (!heuristic_name.compare("manhattan")) {
            _heuristic =
              std::bind(&Heuristic::manhattan, std::placeholders::_1, std::placeholders::_2);
        } else if (!heuristic_name.compare("octogonal")) {
            _heuristic =
              std::bind(&Heuristic::octagonal, std::placeholders::_1, std::placeholders::_2);
        } else {
            goto error;
        }

        _dirs = (conf["allow-diagonals"].asBoolean()) ? 8 : 4;
    }
    return true;

error:
    _dirs = 4;
    if (_heuristic)
        _heuristic = {};
    return false;
}

/*****************************************************************************/
template<typename T>
bool
Impl<T>::run(Graph<T>* world, T* start, T* end) noexcept
{
    _world = world;
    if (nullptr == _world || nullptr == start || nullptr == end)
        return false;

    AStarCell* cur{ nullptr };
    CellsList  open;
    CellsSet   closed;

    open.push_back(start);

    while (!std::empty(open)) {
        auto it{ std::max_element(std::begin(open), std::end(open), [](auto* a, auto* b) {
            return static_cast<AStarCell*>(a)->getScore() >= static_cast<AStarCell*>(b)->getScore();
        }) };
        cur = static_cast<AStarCell*>(*it);

        if (end == cur)
            break;

        closed.insert(cur);
        open.erase(it);

        for (uint i{ 0 }; i < _dirs; ++i) {
            auto neigh{ _world->cell(cur->x() + DIRS[i].first, cur->y() + DIRS[i].second) };
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

    if (cur != end)
        return false;

    while (nullptr != cur) {
        cur->addState(ICell::PATH);
        cur = cur->_parent;
    }

    return true;
}

/*****************************************************************************/
template<typename T>
bool
Impl<T>::_eligible(T* cell) noexcept
{
    if (nullptr == cell)
        return false;

    return !cell->hasState(ICell::WALL);
}

/*****************************************************************************/
std::pair<int, int>
Heuristic::getDelta(AStarCell* s, AStarCell* d) noexcept
{
    return { abs(static_cast<int>(s->x()) - static_cast<int>(d->x())),
             abs(static_cast<int>(s->y()) - static_cast<int>(d->y())) };
}

/*****************************************************************************/
uint
Heuristic::manhattan(AStarCell* s, AStarCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return static_cast<uint>(10 * (delta.first + delta.second));
}

/*****************************************************************************/
uint
Heuristic::euclidean(AStarCell* s, AStarCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return static_cast<uint>(10 * sqrt(pow(delta.first, 2) + pow(delta.second, 2)));
}

/*****************************************************************************/
uint
astar::Heuristic::octagonal(AStarCell* s, AStarCell* d) noexcept
{
    auto delta{ getDelta(s, d) };
    return 10 * (delta.first + delta.second) + (-6) * std::min(delta.first, delta.second);
}

template class Impl<AStarCell>;
}
