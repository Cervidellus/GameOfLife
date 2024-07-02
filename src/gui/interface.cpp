#include <gui\interface.hpp>
#include <presets\modelpresets.hpp>


#include <iostream>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

void Interface::startDraw(
    bool& modelRunning,
    int& desiredModelFPS,
    const int measuredModelFPS) 
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

    ImGui::Begin("Options");
    ImGui::SliderInt("Desired Model FPS", &desiredModelFPS, 1, 120);
    ImGui::Text("Measured FPS: %d", measuredModelFPS);
    if (modelRunning) {
        if (ImGui::Button("Pause Model")) {
            modelRunning = false;
        }
    }
    else {
        if (ImGui::Button("Start Model")) {
            modelRunning = true;
        }
    }
}

void Interface::endDraw(SDL_Renderer* renderer) {
    ImGui::End();
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

//bool Interface::isAnyItemActive() {
//    return ImGui::IsAnyItemActive();
//}
//
//bool Interface::isPointInOverlay(int x, int y)
//{
//    bool pointInOverlay = false;
//    if (x >= imGuiRect_.x && x <= imGuiRect_.x + imGuiRect_.w) {
//        if (y >= imGuiRect_.y && y <= imGuiRect_.y + imGuiRect_.h) pointInOverlay = true;
//    }
//    return pointInOverlay;
//}

Interface::~Interface() {
    if (isInitialized_) {
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}