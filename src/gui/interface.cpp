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
    ImGui::SliderInt("Desired Model FPS", &desiredModelFPS, 1, 1000);
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

Interface::~Interface() {

}