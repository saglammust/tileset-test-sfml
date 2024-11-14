#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"

#include "TileManager.h"

TileManager::TileManager(Vec2 gs, Vec2 dim, const std::string &pathToTexture)
    : m_gridSize(gs), m_worldDimensions(dim)
{
    init(pathToTexture);
}

int TileManager::init(const std::string &pathToTexture)
{
    m_window.create(sf::VideoMode(m_gridSize.x * m_worldDimensions.x, m_gridSize.y * m_worldDimensions.y), "Tilemap Test");
    m_window.setFramerateLimit(60);

    if (!m_tileset.loadFromFile(pathToTexture))
    {
        std::cerr << "Could not load texture file" << std::endl;

        return -1;
    }

    for (uint i = 0; i < (m_tileset.getSize().x / (int)m_gridSize.x) * (m_tileset.getSize().y / (int)m_gridSize.y); i++)
    {
        int tu = i % (m_tileset.getSize().x / (int)m_gridSize.x);
        int tv = i / (m_tileset.getSize().x / (int)m_gridSize.x);

        auto sprite = sf::Sprite(m_tileset, sf::IntRect(tu * m_gridSize.x, tv * m_gridSize.y, m_gridSize.x, m_gridSize.y));
        m_tiles.emplace_back(Tile(sprite, i));
        m_tiles.size();
    }

    uint numOfTile = (m_tileset.getSize().x / (int)m_gridSize.x) * (m_tileset.getSize().y / (int)m_gridSize.y);
    std::srand(unsigned(std::time(nullptr)));
    for (size_t i = 0; i < m_worldDimensions.x * m_worldDimensions.y; i++)
    {
        int p = rand() % numOfTile;
        m_level.emplace_back(p);
    }

    if (!m_tilemap.load(m_tileset, m_gridSize, m_level, m_worldDimensions.x, m_worldDimensions.y))
    {
        std::cerr << "Could not load tileset file" << std::endl;

        return -2;
    }

    if (m_font.loadFromFile("../resources/tech.ttf"))
    {
        m_gridText.setCharacterSize(12);
        m_gridText.setFont(m_font);
    }
    else
        return -3;

    if (!ImGui::SFML::Init(m_window))
    {
        std::cerr << "Failed ImGui initialization" << std::endl;

        return -4;
    }

    return 0;
}

float TileManager::width() const
{
    return static_cast<float>(m_window.getSize().x);
}

float TileManager::height() const
{
    return static_cast<float>(m_window.getSize().y);
}

void TileManager::drawLine(const Vec2 &p1, const Vec2 &p2)
{
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(p1.x, p1.y)),
        sf::Vertex(sf::Vector2f(p2.x, p2.y))};

    m_window.draw(line, 2, sf::Lines);
}

bool TileManager::isRunning()
{
    return m_running & m_window.isOpen(); // maybe '&&' instead of '&'?
}

void TileManager::run()
{
    while (isRunning())
    {
        sUserInput();
        ImGui::SFML::Update(m_window, m_deltaClock.restart());
        sGUI();
        sRender();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void TileManager::quit()
{
    m_running = false;
    m_window.close();
}

void TileManager::updateOnMouse(Vec2 &mPos)
{
    if (mPos.x > 0 && mPos.y > 0 && mPos.x < m_window.getSize().x && mPos.y < m_window.getSize().y)
    {
        int x = (int)(mPos.x / m_gridSize.x);
        int y = (int)(mPos.y / m_gridSize.y);

        m_level[y * m_worldDimensions.x + x] = m_brush;

        m_tilemap.load(m_tileset, m_gridSize, m_level, m_worldDimensions.x, m_worldDimensions.y);
    }
}

void TileManager::sGUI()
{
    ImGui::Begin("Scene Properties");

    if (ImGui::BeginTabBar("MyTabBar"))
    {

        if (ImGui::BeginTabItem("Debug"))
        {
            ImGui::Checkbox("Draw Grid (G)", &m_drawGrid);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Tilesets"))
        {
            // TODO:
            /*
            for (auto &tileset : m_game->assets().getTilesets())
            {
                ImGui::Text(tileset.first.c_str());
                for (auto const &tile : tileset.second.getAllSprites())
                {
                    if (ImGui::ImageButton(tile))
                    {
                    }
                    ImGui::SameLine();
                }
            }
            */

            ImGui::Text("Individual Tiles in the Tileset");
            for (auto &tile : m_tiles)
            {
                ImGui::PushID(std::to_string(tile.index).c_str());
                if (ImGui::ImageButton(tile.sprite))
                {
                    m_brush = tile.index;
                    std::cout << "index:" << tile.index << std::endl;
                }
                ImGui::PopID();
                ImGui::SameLine();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void TileManager::sRender()
{
    m_window.clear();
    m_window.draw(m_tilemap);
    // draw the grid so that can easily debug
    if (m_drawGrid)
    {
        float leftX = m_window.getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);
        float topY = m_window.getView().getCenter().y - height() / 2.0;
        float bottomY = topY + height() + m_gridSize.y;
        float nextGridY = topY - ((int)topY % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, topY), Vec2(x, bottomY));
        }

        for (float y = nextGridY; y < bottomY; y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, y), Vec2(rightX, y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                int w = width();
                int h = height();

                std::string xCell = std::to_string(
                    ((((int)x % w) + w) % w) / (int)m_gridSize.x);
                std::string yCell = std::to_string(
                    ((((int)y % h) + h) % h) / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, y + 2);
                m_window.draw(m_gridText);
            }
        }
    }
}

void TileManager::sUserInput()
{
    // handle events
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);

        if (event.type == sf::Event::Closed)
        {
            quit();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            std::cout << "Key Pressed with code = " << event.key.code << "\n";
            if (event.key.code == sf::Keyboard::Escape)
            {
                quit();
            }
        }

        // this line ignores mouse events if ImGui is the thing clicked
        if (ImGui::GetIO().WantCaptureMouse)
        {
            continue;
        }
        // mouse event below

        if (event.type == sf::Event::MouseButtonPressed)
        {
            Vec2 pos = Vec2(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            m_isPainting = true;
            updateOnMouse(pos);
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            m_isPainting = false;
        }
        if (event.type == sf::Event::MouseMoved && m_isPainting)
        {
            Vec2 pos = Vec2(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            updateOnMouse(pos);
        }
    }
}