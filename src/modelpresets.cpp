#include <modelpresets.hpp>

#include <iostream>
#include <SDL2/SDL.h>

//Maybe I should have this in core? That way I could set all of the members at the same time
//SDL_Surface* ModelPresets::generateModelPresetSurface(const ModelParameters& params){
//	//First set the members to correspond with the parameters
//    int surfaceWidth = (params.modelWidth >= params.minWidth) ? params.modelWidth : params.minWidth;
//    int surfaceHeight = (params.modelHeight >= params.minHeight) ? params.modelHeight : params.minHeight;
//
//	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, surfaceWidth, surfaceHeight, 1, SDL_PIXELFORMAT_INDEX8);
//    if (surface == nullptr) {
//        std::cout << "Error creating surface: " << SDL_GetError() << std::endl;
//        return nullptr;
//    }
//    //Set the color palette
//    const SDL_Color colors[2] = {
//		{0, 0, 0, 255},
//		{255, 255, 255, 255}
//	};
//    SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);
//
//	if (params.random) {
//		srand(static_cast<unsigned>(time(nullptr)));
//		for (int row = 0; row < surfaceHeight; row++) {
//			for (int column = 0; column < surfaceWidth; column++) {
//				*((Uint8*)surface->pixels + row * surface->pitch + column) = (rand() < params.fillFactor * (float)RAND_MAX) ? 1 : 0;
//			}
//		}
//	}
//
//
//	return surface;
//}

//SDL_Surface* Core::generateRandomModelSurface() {
//    //Create and single bit surface to represent black and white values
//    //SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, modelWidth_, modelHeight_, 1, SDL_PIXELFORMAT_INDEX8);//single bit per pixel image format
//    if (surface == nullptr) {
//        std::cout << "Error creating surface: " << SDL_GetError() << std::endl;
//        return nullptr;
//    }
//    //Set the color palette
//    const SDL_Color colors[2] = {
//        {0, 0, 0, 255},
//        {255, 255, 255, 255}
//    };
//    SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);
//
//    //Fill with random data
//    srand(static_cast<unsigned>(time(nullptr)));
//    for (int row = 0; row < modelHeight_; row++) {
//        for (int column = 0; column < modelWidth_; column++) {
//            *((Uint8*)surface->pixels + row * surface->pitch + column) = (rand() < fillFactor_ * (float)RAND_MAX) ? 1 : 0;
//        }
//    }
//
//    return surface;
//}