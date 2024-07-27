#include <core.hpp>

//#define SDL_MAIN_HANDLED
#include "../src/model/LifeQuadTree.hpp"

int main(int argc, char** argv){
    Core game;
    if(!game.run()) return -1;

    return 0;
}