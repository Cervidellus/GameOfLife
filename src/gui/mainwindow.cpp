#include "mainwindow.hpp"

#include <iostream>

#include <imgui.h>"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>


MainWindow::MainWindow(){}

void MainWindow::initialize(std::string windowName)
{
	sdlWindow = SDL_CreateWindow(
		windowName.c_str(),
		//SDL_WINDOWPOS_CENTERED,
		//SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_OPENGL */
	);

	sdlRenderer = SDL_CreateRenderer(
		sdlWindow,
		nullptr
	);
	SDL_SetRenderVSync(sdlRenderer, 1);
	SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void MainWindow::processEvent(SDL_Event& event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

MainWindowSize MainWindow::getSize()
{
	MainWindowSize size;
	SDL_GetWindowSize(sdlWindow, &size.width, &size.height);
	return size;
}

void MainWindow::drawTexture(SDL_Texture* texture, SDL_FRect destination)
{
	SDL_RenderTexture(sdlRenderer, texture, NULL, &destination);
}


void MainWindow::clear() {
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
}

void MainWindow::renderPresent()
{
	SDL_RenderPresent(sdlRenderer);
}

MainWindow::~MainWindow() 
{
}