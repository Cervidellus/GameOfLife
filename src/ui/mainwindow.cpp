#include "mainwindow.hpp"

#include <SDL2/SDL.h>

MainWindow::MainWindow(
	const char* windowName
	//const uint32_t windowFlags,
	//const uint32_t rendererFlags
) : window_(SDL_CreateWindow(windowName, 
							SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 
							1280, 
							720, 
							SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED), 
			SDL_DestroyWindow),
	renderer_(SDL_CreateRenderer(window_.get(), 
								-1, 
								SDL_RENDERER_ACCELERATED),
			  SDL_DestroyRenderer)
{
}

MainWindowSize MainWindow::getSize()
{
	MainWindowSize size;
	SDL_GetWindowSize(window_.get(), &size.width, &size.height);
	return size;
}

MainWindow::~MainWindow() 
{
	renderer_.reset();
	window_.reset();
}