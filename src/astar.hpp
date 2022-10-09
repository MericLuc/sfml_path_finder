/**
 * @file astar.hpp
 * @brief A-star algorithm implementation
 * @author lhm
 */

#ifndef SRC_ASTAR_HPP
#define SRC_ASTAR_HPP

#include <functional>
#include <list>
#include <set>

namespace JSON {
class Object;
}

class Graph;
class Node;

namespace AStar {
struct coords
{
    int x, y;

    bool operator==(const coords&) noexcept;
};

using uint = unsigned int;
using HeuristicFunction = std::function<uint(const coords&, const coords&)>;

using NodesList = std::list<Node*>;
using NodesArr = std::vector<Node*>;

struct Node
{
    uint   G, H;
    coords coordinates;
    Node*  parent;

    Node(coords coord_, Node* parent_ = nullptr);
    uint getScore() const noexcept;
};

class Generator
{
public:
    Generator();
    [[maybe_unused]] virtual bool configure(const JSON::Object&) noexcept;
    virtual NodesList             run(Graph*) noexcept;

protected:
    bool _eligible(Node*) noexcept;

private:
    Graph*   _world{ nullptr };
    NodesArr _walls{};

    HeuristicFunction _heuristic;
    uint              _dirs;
};

class Heuristic
{
public:
    static uint manhattan(const coords&, const coords&) noexcept;
    static uint euclidean(const coords&, const coords&) noexcept;
    static uint octagonal(const coords&, const coords&) noexcept;

private:
    static coords getDelta(const coords&, const coords&) noexcept;
};
}

#endif // SRC_ASTAR_HPP
