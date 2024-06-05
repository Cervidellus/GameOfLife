#include "gui.hpp"

#include <imgui.h>

GUI::GUI() :
	context_(ImGui::CreateContext())
{
	//This ImGui context is shared by all windows
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	mainWindow.initialize();
};

//void GUI::shutdown() {
//};

GUI::~GUI() 
{
	ImGui::DestroyContext();
};