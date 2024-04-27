/*main will take in command line arguments before
initializing and running the core application*/
#include <core.hpp>

int main(int argc, char** argv){
    Core game;
    if(!game.run()) return -1;

    return 0;
}