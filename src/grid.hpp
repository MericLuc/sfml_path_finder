/**
 * @file grid.hpp
 * @brief The Grid rendered
 * @author lhm
 */

#ifndef SRC_GRID_HPP
#define SRC_GRID_HPP

// Standard headers
#include <memory>
#include <type_traits>

// External headers
#include <SFML/Graphics.hpp>

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
        PATH = 1 << 4,
        SELECTED = 1 << 5
    };

public:
    ICell(uint, uint) noexcept;
    virtual ~ICell() noexcept = default;

    virtual void clear(void) noexcept { _state = EMPTY; }
    virtual void clean(void) noexcept { _state &= ~PATH; }

    auto x(void) const noexcept { return _x; }
    auto y(void) const noexcept { return _y; }

    bool hasState(int st) const noexcept { return _state & st; }
    auto getState(void) const noexcept { return _state; }
    auto setState(int st) noexcept { _state = st; }
    auto addState(State st) noexcept { _state |= st; }
    auto remState(State st) noexcept { _state &= ~st; }

protected:
    uint _x, _y;
    int  _state{ EMPTY };
};

/*****************************************************************************/
class PathCell : public ICell
{
public:
    PathCell(uint, uint, PathCell* parent = nullptr);
    virtual ~PathCell() noexcept = default;

    virtual void clear(void) noexcept override;
    virtual void clean(void) noexcept override;

    uint getScore() const noexcept;

    uint      _G{ 0 }, _H{ 0 };
    PathCell* _parent;
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
        Graph::resize(width, height);
    }

    virtual ~Graph() noexcept = default;

    void clear(void) noexcept
    {
        for (auto& cell : _grid)
            cell.clear();
    }
    void clean(void) noexcept
    {
        for (auto& cell : _grid)
            cell.clean();
    }

    void resize(size_t width, size_t height) noexcept
    {
        _width = width;
        _height = height;

        _grid.clear();
        _grid.reserve(_width * _height);
        for (size_t i{ 0 }; i < _height; ++i)
            for (size_t j{ 0 }; j < _width; ++j)
                _grid.push_back(T(j, i));
    }

    auto getWidth(void) const noexcept { return _width; }
    auto getHeight(void) const noexcept { return _height; }

    T* cell(size_t i, size_t j) const noexcept
    {
        return (i < _width && j < _height) ? &_grid.at(i + j * _width) : nullptr;
    }

protected:
    size_t                 _width, _height;
    mutable std::vector<T> _grid;
};

namespace ui {

/*****************************************************************************/
class Cell
  : public PathCell
  , public sf::RectangleShape
{
    friend class Grid;

public:
    Cell(uint, uint) noexcept;
    virtual ~Cell() noexcept = default;

protected:
    void update(void) noexcept;
};

/*****************************************************************************/
class Grid
  : public Graph<ui::Cell>
  , public sf::Drawable
{
public:
    Grid() noexcept;
    virtual ~Grid() noexcept = default;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif // SRC_GRID_HPP
