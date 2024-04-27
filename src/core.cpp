#include <core.hpp>

#include <iostream>

#include <SDL2/SDL.h>

Core::Core() {
}

bool Core::run() {
    if(!init()) return false;

    while (running_) {
        processEvents();
        update();
        render();
    }

    return true;
}

bool Core::init() {
    //Temporary surface 
    surface_ = SDL_LoadBMP("resources/fern.bmp");

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        // If SDL fails to initialize, print an error message
        std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
        running_ = false;
        return false;
    }
    std::cout << "SDL2 video initialized." << std::endl;
    running_ = true;
    window_ = SDL_CreateWindow("GameOfLife", 50, 50, 800, 600, SDL_WINDOW_RESIZABLE);
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    return true;
}

void Core::processEvents() {
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
    
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer_, &windowWidth, &windowHeight);
    SDL_Rect destinationRect{0, 0, windowWidth, windowWidth};

    texture_ = SDL_CreateTextureFromSurface(renderer_, surface_);

    SDL_RenderCopy(renderer_, texture_, NULL, &destinationRect);
    SDL_RenderPresent(renderer_);
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
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_DestroyTexture(texture_);
    SDL_FreeSurface(surface_);

    SDL_Quit();
}