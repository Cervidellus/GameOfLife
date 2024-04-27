#include <core.hpp>

#include <iostream>

#include <SDL.h>

Core::Core() {
    // Initialize the core application
    // renderer_ = 
}

bool Core::run() {
    // Initialize the game
    if(!init()) return false;

    while (running_) {
        processEvents();
        update();
        render();

        //temporary sleep in order to show the window
        SDL_Delay(5000);
        running_ = false;
    }

    return true;
}

bool Core::init() {
    // Initialize the game
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        // If SDL fails to initialize, print an error message
        std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
        running_ = false;
        return false;
    }
    std::cout << "SDL2 video initialized." << std::endl;
    running_ = true;
    window_ = SDL_CreateWindow("GameOfLife", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
    return true;
}

void Core::processEvents() {
    // If escape pressed, stop the game
}

void Core::update() {
    // Update the matrix
}

void Core::render() {
}

Core::~Core() {
    //Because SDL is a C library, we need to call SDL methods to clean up
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}