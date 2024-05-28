#include <interface.hpp>
#include <iostream>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

Interface::Interface() {
    std::cout << "Interface created" << std::endl;
}

bool Interface::init(
    SDL_Window* window, 
    SDL_Renderer* renderer,
    std::function<void()> generateModelButtonCallback) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    if(!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) return false;
    if(!ImGui_ImplSDLRenderer2_Init(renderer)) return false;

    generateModelCallback_ = std::make_unique<std::function<void()>>(std::move(generateModelButtonCallback));

	return true;
}

//void Interface::setGenerateModelCallback(std::function<void()> callback) {
//    generateModelCallback_ = std::make_unique<std::function<void()>>(std::move(callback));
//}

void Interface::render(
    int& modelFPS,
    const int measuredModelFPS,
    bool& modelRunning,
    float& fillFactor,
    int& modelWidth,
    int& modelHeight,
    int& rule1,
    int& rule3,
    int& rule4)
{
    //IMGUI
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Options");
    ImGui::SliderInt("Desired Model FPS", &modelFPS, 1, 120);
    ImGui::Text("Measured FPS: %d", measuredModelFPS);
    if (modelRunning) {
        if (ImGui::Button("Pause Model")) {
            modelRunning = false;
        }
    }
    else {
        ImGui::SliderFloat("Model Fill Factor", &fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        if (ImGui::Button("Generate Model")) {
            if(generateModelCallback_) (*generateModelCallback_)();
        }
        ImGui::SameLine();
        if (ImGui::Button("Start Model")) {
            modelRunning = true;
        }
    }

    ImGuiInputTextFlags modelInputFlags = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

    //TODO:: limit to positive values
    //TODO:: ensure that input is 4-byte aligned
    ImGui::InputInt("Width", &modelWidth, 100, 100, modelInputFlags);
    ImGui::InputInt("Height", &modelHeight, 100, 100, modelInputFlags);

    ImGuiInputTextFlags ruleInputFlags = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

    if (ImGui::InputInt("Conway Rule 1 Cutoff", &rule1, 1, 1))
    {
        if (rule1 < 0) rule1 = 0;
        if (rule1 > 8) rule1 = 8;
        if (rule1 >= rule3) rule1 = rule3 - 1;
    };
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has fewer than this many neighbors, it dies.");

    if (ImGui::InputInt("Conway Rule 3 Cutoff", &rule3, 1, 1))
    {
        if (rule3 < 1) rule3 = 1;
        if (rule3 > 8) rule3 = 8;
        if (rule3 <= rule1) rule3 = rule1 + 1;
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has more than this many neighbors, it dies.");

    if (ImGui::InputInt("Conway Rule 4", &rule4, 1, 1))
    {
        if (rule4 < 0) rule4 = 0;
        if (rule4 > 8) rule4 = 8;
    };
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a dead cell has exactly this many neighbors, it becomes alive.");

    ImGui::End();
    ImGui::Render();
}


Interface::~Interface() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}