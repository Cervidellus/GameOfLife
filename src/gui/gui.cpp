#include "gui.hpp"

#include <iostream>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <SDL3/sdl_pixels.h>
#include <SDL3/SDL_video.h>


GUI::GUI() {};

bool GUI::initialize(
	std::string windowTitle
)
{
	mainWindow.initialize(windowTitle.c_str());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	io.Fonts->Build();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	std::cout << SDL_GetCurrentVideoDriver();
	ImGui_ImplSDL3_InitForSDLRenderer(mainWindow.sdlWindow, mainWindow.sdlRenderer);
	//SDL_GetCurrentVideoDriver() is getting back null.
	if (!ImGui_ImplSDL3_InitForSDLRenderer(mainWindow.sdlWindow, mainWindow.sdlRenderer)) return false;//fails... mainwindow does not have a renderer
	if (!ImGui_ImplSDLRenderer3_Init(mainWindow.sdlRenderer)) return false;

	return true;
};

void GUI::shutdown()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

GUI::~GUI() 
{
	
};