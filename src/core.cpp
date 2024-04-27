#include <core.hpp>

#include <iostream>

// #include <SDL.h>
#include <SDL2/SDL.h>

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
    window_ = SDL_CreateWindow("GameOfLife", 50, 50, 800, 600, SDL_WINDOW_SHOWN);
    return true;
}

void Core::processEvents() {
    // If escape pressed, stop the game
    SDL_Event event;

    while(SDL_PollEvent(&event)) {

        switch(event.type)
        {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_KEYDOWN:
                handleSDL_KEYDOWN(event);
                break;
        }
    }
}

void Core::update() {
    // Update the matrix
}

void Core::render() {
}

void Core::handleSDL_KEYDOWN(SDL_Event& event) {
    switch(event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
            running_ = false;
            std::cout << "Escape key pressed: Exiting application." << std::endl;
            break;
        default:
            break;
    }
}

Core::~Core() {
    //Because SDL is a C library, we need to call SDL methods to clean up
    // SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}