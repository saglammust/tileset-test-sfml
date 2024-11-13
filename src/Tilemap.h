#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const sf::Texture &texture, Vec2 tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};