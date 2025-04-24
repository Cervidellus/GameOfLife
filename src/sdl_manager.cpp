#include "sdl_manager.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

SDLManager::SDLManager() {
	initialize_();
}

bool SDLManager::initialize_() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		isInitialized_ = true;
		std::cout << "SDL3 video initialized..." << std::endl;
	}
	else {
		char errorMessage[256];
		SDL_strlcpy(errorMessage, SDL_GetError(), sizeof(errorMessage));
		std::cerr << "SDL_Init Error: " << errorMessage << std::endl;
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