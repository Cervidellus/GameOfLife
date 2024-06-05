#include "mainwindow.hpp"

#include "imgui.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>

MainWindow::MainWindow(
	const char* windowName
) : window_(SDL_CreateWindow(windowName, 
							SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 
							1280, 
							720, 
							SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED)),
	renderer_(SDL_CreateRenderer(window_, 
								-1, 
								SDL_RENDERER_ACCELERATED))
{
	//Mainwindow is getting created BEFORE the gui is finished being constructed
	//ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
	//ImGui_ImplSDLRenderer2_Init(renderer_);
}

void MainWindow::initialize()
{
	ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
	ImGui_ImplSDLRenderer2_Init(renderer_);
}

void MainWindow::processEvent(SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

MainWindowSize MainWindow::getSize()
{
	MainWindowSize size;
	SDL_GetWindowSize(window_, &size.width, &size.height);
	return size;
}

MainWindow::~MainWindow() 
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}