#include <core.hpp>

//#define SDL_MAIN_HANDLED
#include "../src/model/LifeQuadTree.hpp"
#include "../src/model/VectorGrid.hpp"

int main(int argc, char** argv){

    auto myGrid = VectorGrid(100, 100);

    Core game;
    if(!game.run()) return -1;

    return 0;
}