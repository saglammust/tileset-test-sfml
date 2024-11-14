#pragma once

#include <SFML/Graphics.hpp>

#include "Tilemap.h"
#include "Vec2.h"

struct Tile
{
    sf::Sprite sprite;
    uint index;
};

class TileManager
{
    sf::RenderWindow m_window;
    sf::Clock m_deltaClock;

    Vec2 m_gridSize;
    Vec2 m_worldDimensions;

    bool m_drawGrid = false;
    bool m_running = true;
    bool m_isPainting = false;

    std::vector<int> m_level;
    TileMap m_tilemap;
    int m_brush = 1;

    sf::Text m_gridText;
    sf::Font m_font;
    sf::Texture m_tileset;
    std::vector<Tile> m_tiles;

public:
    TileManager(Vec2 gs, Vec2 dim, const std::string &pathToTexture);

    int init(const std::string &pathToTexture);
    void sRender();
    void sGUI();
    void sUserInput();

    void run();
    void quit();
    void updateOnMouse(Vec2 &mousePos);

    float width() const;
    float height() const;
    void drawLine(const Vec2 &p1, const Vec2 &p2);
    bool isRunning();
};