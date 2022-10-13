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

// Project's headers
#include <env/graph.hpp>

// External headers
#include <SFML/Graphics.hpp>

namespace graphics {

/*****************************************************************************/
template<typename T>
class Grid : public sf::Drawable
{
public:
    Grid(env::Graph<T>*) noexcept;
    virtual ~Grid() noexcept = default;

    void setGraph(env::Graph<T>*) noexcept;
    void setCursor(T*) noexcept;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void updateCellStyle(uint i, uint j) const noexcept;
    virtual void updateCell(uint i, uint j, sf::Vector2f pos, sf::Vector2f size) const noexcept;

protected:
    mutable std::vector<sf::Vertex> _vertexes;
    mutable std::vector<sf::Vertex> _grid;
    env::Graph<T>*                  _graph{ nullptr };
    T*                              _cursor{ nullptr };
};

}

#endif // SRC_GRID_HPP
