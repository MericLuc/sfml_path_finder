/**
 * @file grid.hpp
 * @brief The Grid rendered
 * @author lhm
 */

#ifndef SRC_GRID_HPP
#define SRC_GRID_HPP

// Standard headers
#include <memory>

// External headers
#include <SFML/Graphics.hpp>

namespace ui {
class Cell : public sf::RectangleShape
{
    friend class Grid;

public:
    enum State
    {
        EMPTY = 0,
        WALL = 1 << 1,
        START_CELL = 1 << 2,
        END_CELL = 1 << 3,
        PATH = 1 << 4,    // Cell belongs to the path
        SELECTED = 1 << 5 // under cursor
    };

    Cell() noexcept = default;
    virtual ~Cell() noexcept = default;

    bool hasState(int st) const noexcept { return _state & st; }
    int  getState(void) const noexcept { return _state; }
    void setState(int st) noexcept { _state = st; }
    void addState(State st) noexcept { _state |= st; }
    void remState(State st) noexcept { _state ^= st; }

protected:
    void update(void) noexcept;

private:
    int _state{ EMPTY };
};

/*****************************************************************************/
class Grid : public sf::Drawable
{
public:
    Grid(size_t width = 50, size_t height = 50) noexcept;
    virtual ~Grid() noexcept = default;

    virtual void clean(void) noexcept;
    virtual void resize(size_t width, size_t height) noexcept;

    auto  getWidth(void) const noexcept { return _width; }
    auto  getHeight(void) const noexcept { return _height; }
    Cell* cell(size_t i, size_t j) noexcept
    {
        return (i < _width && j < _height) ? &_grid[i + j * _width] : nullptr;
    }

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    size_t                    _width, _height;
    mutable std::vector<Cell> _grid;
};

}

#endif // SRC_GRID_HPP
