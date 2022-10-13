/**
 * @file grid.hpp
 * @brief The Grid rendered
 * @author lhm
 */

#ifndef SRC_ENV_GRAPH_HPP
#define SRC_ENV_GRAPH_HPP

// Standard headers
#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

typedef unsigned int uint;

namespace env {

using Dims = std::pair<size_t, size_t>;

/*****************************************************************************/
class ICell
{
public:
    enum State
    {
        EMPTY = 0,
        WALL = 1 << 1,
        START_CELL = 1 << 2,
        END_CELL = 1 << 3,
        PATH = 1 << 4
    };

public:
    ICell(uint, uint) noexcept;
    virtual ~ICell() noexcept = default;

    [[maybe_unused]] virtual bool clear(void) noexcept;
    [[maybe_unused]] virtual bool clean(void) noexcept;

    auto x(void) const noexcept { return _x; }
    auto y(void) const noexcept { return _y; }

    bool hasState(int st) const noexcept { return _state & st; }
    int  getState(void) const noexcept { return _state; }

    [[maybe_unused]] bool setState(int st) noexcept;
    [[maybe_unused]] bool addState(State st) noexcept;
    [[maybe_unused]] bool remState(State st) noexcept;

protected:
    uint _x, _y;
    int  _state{ EMPTY };
};

/*****************************************************************************/
class AStarCell : public ICell
{
public:
    AStarCell(uint, uint, AStarCell* parent = nullptr);
    virtual ~AStarCell() noexcept = default;

    [[maybe_unused]] virtual bool clear(void) noexcept override;
    [[maybe_unused]] virtual bool clean(void) noexcept override;

    uint getScore() const noexcept;

    uint       _G{ 0 }, _H{ 0 };
    AStarCell* _parent;
};

/*****************************************************************************/
template<typename T, typename = std::enable_if_t<std::is_base_of_v<ICell, T>>>
class Graph
{
public:
    Graph(size_t width = 50, size_t height = 50) noexcept
      : _width{ width }
      , _height{ height }
    {
        resize(width, height);
    }

    virtual ~Graph() noexcept = default;

    [[maybe_unused]] bool clear(void) noexcept
    {
        bool ret{ false };
        std::for_each(std::begin(_data), std::end(_data), [&ret](auto& c) { ret |= c.clear(); });
        return ret;
    }
    [[maybe_unused]] bool clean(void) noexcept
    {
        bool ret{ false };
        std::for_each(std::begin(_data), std::end(_data), [&ret](auto& c) { ret |= c.clean(); });
        return ret;
    }

    virtual void resize(size_t width, size_t height) noexcept
    {
        _width = width;
        _height = height;

        _data.clear();
        _data.reserve(_width * _height);
        for (size_t i{ 0 }; i < _height; ++i)
            for (size_t j{ 0 }; j < _width; ++j)
                _data.push_back(T(j, i));
    }

    auto getWidth(void) const noexcept { return _width; }
    auto getHeight(void) const noexcept { return _height; }
    Dims getSize(void) const noexcept { return { _width, _height }; }

    T* cell(size_t i, size_t j) const noexcept
    {
        return (i < _width && j < _height) ? &_data.at(i + j * _width) : nullptr;
    }

protected:
    size_t                 _width, _height;
    mutable std::vector<T> _data;
};

}

#endif // SRC_ENV_GRAPH_HPP
