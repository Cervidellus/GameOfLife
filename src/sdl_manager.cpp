#include "sdl_manager.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

SDLManager::SDLManager() {
	initialize_();
}

bool SDLManager::initialize_() {
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		isInitialized_ = true;
		std::cout << "SDL2 video initialized..." << std::endl;
	}
	else {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		isInitialized_ = false;
	}
	return isInitialized_;
}

void SDLManager::shutdown() {
	SDL_Quit();
	isInitialized_ = false;
}

SDLManager::~SDLManager() {
	shutdown();
	std::cout << "SDLManager destroyed" << std::endl;
}