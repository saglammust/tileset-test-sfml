#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "Tilemap.h"
#include "TileManager.h"

int main()
{
    TileManager tm({64, 64}, {20, 12}, "../resources/ground_hot_lush_01.png");
    tm.run();

    return 0;
}