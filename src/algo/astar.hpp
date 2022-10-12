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
#include <env/graph.hpp>

namespace JSON {
class Object;
}

namespace astar {

template<typename T>
using HeuristicFunction = std::function<uint(T*, T*)>;

/*****************************************************************************/
template<typename T>
class AbstractImpl
{
public:
    [[maybe_unused]] virtual bool configure(const JSON::Object&) noexcept = 0;
    [[maybe_unused]] virtual bool run(env::Graph<T>*, T* start, T* end) noexcept = 0;
};

/*****************************************************************************/
template<typename T>
class Impl : public AbstractImpl<T>
{
public:
    Impl() noexcept;
    virtual ~Impl() noexcept = default;

    [[maybe_unused]] virtual bool configure(const JSON::Object&) noexcept override;
    [[maybe_unused]] virtual bool run(env::Graph<T>*, T* start, T* end) noexcept override;

protected:
    virtual bool _eligible(T*) noexcept;

private:
    env::Graph<T>* _world{ nullptr };

    HeuristicFunction<T> _heuristic;
    uint                 _dirs;

    static const std::vector<std::pair<int, int>> DIRS;
};

/*****************************************************************************/
class Heuristic
{
public:
    static uint manhattan(env::AStarCell*, env::AStarCell*) noexcept;
    static uint euclidean(env::AStarCell*, env::AStarCell*) noexcept;
    static uint octagonal(env::AStarCell*, env::AStarCell*) noexcept;

private:
    static std::pair<int, int> getDelta(env::AStarCell*, env::AStarCell*) noexcept;
};
}

#endif // SRC_ASTAR_HPP
