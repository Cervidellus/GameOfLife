#include "gui.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/sdl_pixels.h>


GUI::GUI() {};

bool GUI::initialize(
	std::string windowTitle,
	std::function<void(ModelParameters presetParameters)> presetCallback
)
{
	mainWindow.initialize(windowTitle.c_str());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	if (!ImGui_ImplSDL2_InitForSDLRenderer(mainWindow.sdlWindow, mainWindow.sdlRenderer)) return false;
	if (!ImGui_ImplSDLRenderer2_Init(mainWindow.sdlRenderer)) return false;

	interface.init(
		mainWindow.sdlWindow,
		mainWindow.sdlRenderer,
		presetCallback,
		[&]() {return mainWindow.getTextureFromWindow(SDL_PIXELFORMAT_RGBA8888); }
	);

	return true;
};

GUI::~GUI() 
{
	ImGui::DestroyContext();
};