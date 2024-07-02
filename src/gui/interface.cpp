#include <gui\interface.hpp>
#include <presets\modelpresets.hpp>


#include <iostream>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>


//Interface::Interface() {
//    std::cout << "Interface created" << std::endl;
//}

//bool Interface::init(
//    SDL_Window* window, 
//    SDL_Renderer* renderer
//    //std::function<void(ModelParameters presetParameters)> presetCallback, 
//    //std::function<SDL_Texture* ()> getWindowTextureCallback
//)
//{
//    //presetCallback_ = std::make_unique<std::function<void(ModelParameters presetParameters)>>(std::move(presetCallback));
//    //getWindowTextureCallback_ = std::make_unique<std::function<SDL_Texture*()>>(std::move(getWindowTextureCallback));
//
//    isInitialized_ = true;
//	return true;
//}

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
    //Grab the position and size of the rendered area. Seem to give a size, but not the position?
    //Need to offset dimensions by window position.
    //auto imGuiMin = ImGui::GetWindowContentRegionMin();
    auto imGuiMax = ImGui::GetWindowContentRegionMax();
    auto imGuiWindowPos = ImGui::GetWindowPos();
    imGuiRect_ = SDL_Rect{ (int)imGuiWindowPos.x, (int)imGuiWindowPos.y, (int)imGuiMax.x, (int)imGuiMax.y };

    ImGui::End();
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

bool Interface::isPointInOverlay(int x, int y)
{
    bool pointInOverlay = false;
    if (x >= imGuiRect_.x && x <= imGuiRect_.x + imGuiRect_.w) {
        if (y >= imGuiRect_.y && y <= imGuiRect_.y + imGuiRect_.h) pointInOverlay = true;
    }
    std::cout << pointInOverlay << " mousepos:" << x << " imGui.x" << imGuiRect_.x << " imGui.w" << imGuiRect_.w << "\n";

    return pointInOverlay;
}

Interface::~Interface() {
    if (isInitialized_) {
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}