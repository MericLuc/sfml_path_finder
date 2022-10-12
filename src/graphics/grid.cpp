/**
 * @file app.cpp
 * @brief Implementation of \a app.hpp
 * @author lhm
 */

// Standard headers

// Project headers
#include "grid.hpp"

// External libs
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace env;

namespace graphics {

/*****************************************************************************/
template<typename T>
Grid<T>::Grid(Graph<T>* graph) noexcept
  : _graph{ graph }
{
    update();
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::setGraphe(Graph<T>* graph) noexcept
{
    _graph = graph;
    update();
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::setCursor(T* cursor) noexcept
{
    _cursor = cursor;
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::update() noexcept
{
    if (nullptr != _graph)
        _vertexes =
          std::vector<sf::Vertex>(_graph->getWidth() * _graph->getHeight() * 4, sf::Vertex());
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    static double cell_width{ 0 };
    static double cell_height{ 0 };

    bool       updt{ false };
    const auto new_width{ target.getSize().x / static_cast<double>(_graph->getWidth()) };
    const auto new_height{ target.getSize().y / static_cast<double>(_graph->getHeight()) };

    if (new_width != cell_width || new_height != cell_height) {
        cell_width = new_width;
        cell_height = new_height;
        updt = true;
    }

    for (uint i{ 0 }; i < _graph->getWidth(); ++i)
        for (uint j{ 0 }; j < _graph->getHeight(); ++j) {
            if (updt)
                updateCell(i,
                           j,
                           Vector2f(cell_width * i, cell_height * j),
                           Vector2f(cell_width, cell_height));

            updateCellStyle(i, j);
        }

    target.draw(_vertexes.data(), std::size(_vertexes), sf::Quads, states);

    std::for_each(
      std::begin(_vertexes), std::end(_vertexes), [](auto& v) { v.color = sf::Color::Black; });

    for (uint i{ 0 }; i < _graph->getHeight(); ++i)
        target.draw(_vertexes.data() + 4 * _graph->getWidth() * i,
                    4 * _graph->getWidth(),
                    sf::LinesStrip,
                    states);

    if (nullptr != _cursor) {
        sf::RectangleShape cursor(Vector2f(cell_width, cell_height));
        cursor.setOutlineColor(Color::Green);
        cursor.setOutlineThickness(-3.f);
        cursor.setPosition(Vector2f(cell_width * _cursor->x(), cell_height * _cursor->y()));

        target.draw(cursor, states);
    }
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::updateCellStyle(uint i, uint j) const noexcept
{
    auto      st{ _graph->cell(i, j)->getState() };
    auto      idx{ (i + j * _graph->getWidth()) * 4 };
    sf::Color color{ 200, 200, 200, 250 };

    if (st & (ICell::START_CELL | ICell::END_CELL)) {
        color = Color(220, 20, 20, 250);
    } else if (st & ICell::PATH) {
        color = Color(32, 32, 228);
    } else if (st & ICell::WALL) {
        color = Color::Black;
    }

    for (auto it{ 0 }; it < 4; ++it)
        _vertexes[idx + it].color = color;
}

/*****************************************************************************/
template<typename T>
void
Grid<T>::updateCell(uint i, uint j, Vector2f pos, Vector2f size) const noexcept
{
    auto idx{ (i + j * _graph->getWidth()) * 4 };

    _vertexes[idx].position = { pos.x, pos.y };
    _vertexes[idx + 1].position = { pos.x, pos.y + size.y };
    _vertexes[idx + 2].position = { pos.x + size.x, pos.y + size.y };
    _vertexes[idx + 3].position = { pos.x + size.x, pos.y };
}

template class Grid<AStarCell>;

}
