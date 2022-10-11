/**
 * @file astar.hpp
 * @brief A-star algorithm implementation
 * @author lhm
 */

#ifndef SRC_ASTAR_HPP
#define SRC_ASTAR_HPP

// Standard headers
#include <functional>
#include <list>
#include <set>

// Project's headers
#include <grid.hpp>

namespace JSON {
class Object;
}

namespace astar {

/*****************************************************************************/
struct coord
{
    int x, y;
    constexpr coord(int x, int y)
      : x{ x }
      , y{ y }
    {}
    constexpr bool operator==(const coord& o) noexcept { return o.x == x && o.y == y; }
    friend coord   operator+(const coord& lhs, const coord& rhs) noexcept
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }
};

using HeuristicFunction = std::function<uint(PathCell*, PathCell*)>;

using CellsList = std::list<PathCell*>;
using CellsSet = std::set<PathCell*>;

/*****************************************************************************/
class Impl
{
public:
    Impl() noexcept;
    virtual ~Impl() noexcept = default;

    [[maybe_unused]] virtual bool configure(const JSON::Object&) noexcept;
    [[maybe_unused]] virtual bool run(ui::Grid*, PathCell* start, PathCell* end) noexcept;

protected:
    bool _eligible(PathCell*) noexcept;

private:
    ui::Grid* _world{ nullptr };

    HeuristicFunction _heuristic;
    uint              _dirs;

    static const std::vector<coord> DIRS;
};

/*****************************************************************************/
class Heuristic
{
public:
    static uint manhattan(PathCell*, PathCell*) noexcept;
    static uint euclidean(PathCell*, PathCell*) noexcept;
    static uint octagonal(PathCell*, PathCell*) noexcept;

private:
    static coord getDelta(PathCell*, PathCell*) noexcept;
};
}

#endif // SRC_ASTAR_HPP
