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
class PathCell : public ICell
{
public:
    PathCell(uint, uint, PathCell* parent = nullptr);
    virtual ~PathCell() noexcept = default;

    [[maybe_unused]] virtual bool clear(void) noexcept override;
    [[maybe_unused]] virtual bool clean(void) noexcept override;

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

    [[maybe_unused]] bool clear(void) noexcept
    {
        bool ret{ false };
        for (auto& cell : _grid)
            ret |= cell.clear();
        return ret;
    }
    [[maybe_unused]] bool clean(void) noexcept
    {
        bool ret{ false };
        for (auto& cell : _grid)
            ret |= cell.clean();
        return ret;
    }

    virtual void resize(size_t width, size_t height) noexcept
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
class Grid
  : public Graph<PathCell>
  , public sf::Drawable
{
public:
    Grid() noexcept;
    virtual ~Grid() noexcept = default;

    virtual void resize(size_t width, size_t height) noexcept override
    {
        Graph::resize(width, height);
        _vertexes = std::vector<sf::Vertex>(_width * _height * 4, sf::Vertex());
    }

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void updateCellStyle(uint i, uint j) const noexcept;
    virtual void updateCell(uint i, uint j, sf::Vector2f pos, sf::Vector2f size) const noexcept;

protected:
    mutable std::vector<sf::Vertex> _vertexes;
};

}

#endif // SRC_GRID_HPP
