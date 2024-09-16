#include "sdl_manager.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

SDLManager::SDLManager() {
	initialize_();
}

bool SDLManager::initialize_() {
	SDL_Window* window = 
		SDL_CreateWindow(
			"windowName",
			//SDL_WINDOWPOS_CENTERED,
			//SDL_WINDOWPOS_CENTERED,
			1280,
			720,
			SDL_WINDOW_RESIZABLE
		);

	std::cout << &window;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
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