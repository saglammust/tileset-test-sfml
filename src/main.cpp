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
    TileManager tm({32, 32}, {32, 18});
    tm.init();
    tm.run();

    return 0;
}