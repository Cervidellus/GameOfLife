#include <ui\interface.hpp>
#include <presets\modelpresets.hpp>


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
    std::function<void(ModelParameters presetParameters)> presetCallback)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    if(!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) return false;
    if(!ImGui_ImplSDLRenderer2_Init(renderer)) return false;

    presetCallback_ = std::make_unique<std::function<void(ModelParameters presetParameters)>>(std::move(presetCallback));

	return true;
}

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
        if (ImGui::Button("Start Model")) {
			modelRunning = true;
		}
	}


    ImGuiInputTextFlags modelRunningFlag = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;
    
    if (ImGui::CollapsingHeader("Parameters")) {

        //TODO:: limit to positive values
        //TODO:: ensure that input is 4-byte aligned
        ImGui::InputInt("Width", &modelWidth, 100, 100, modelRunningFlag);
        ImGui::InputInt("Height", &modelHeight, 100, 100, modelRunningFlag);

        ImGui::SliderFloat("Model Fill Factor", &fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        //if (ImGui::Button("Generate Model")) {
        //    if(generateModelCallback_) (*generateModelCallback_)(ModelPreset::random);
        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Start Model")) {
        //    modelRunning = true;
        //}

        //ImGuiInputTextFlags ruleInputFlags = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

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

    }

    if (ImGui::CollapsingHeader("Presets"))
    {
        if (ImGui::Button("random")) {
			if(presetCallback_) (*presetCallback_)(ModelPresets::randomParams);
		}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("A randomly generated field to observe Conway's Game of Life.");

        if (ImGui::Button("Swiss Cheese")) {
			if(presetCallback_) (*presetCallback_)(ModelPresets::swissCheeseParams);
		}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Modified rules can produce different results.");

        if (ImGui::Button("Decomposition")) {
			if(presetCallback_) (*presetCallback_)(ModelPresets::decompositionParams);
		}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Modified rules can produce different results.");

        if (ImGui::Button("Blinker")) {
            if (presetCallback_) (*presetCallback_)(ModelPresets::blinkerParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The smallest oscillator in Conway's Game of Life.");

        if (ImGui::Button("Lightweight Spaceship")) {
			if (presetCallback_) (*presetCallback_)(ModelPresets::lightweightSpaceshipParams);
		}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The smallest orthoganal spaceship in Conway's Game of Life.");

        if (ImGui::Button("Blocker")) {
            if (presetCallback_) (*presetCallback_)(ModelPresets::blockerParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Blocker.");

        if (ImGui::Button("Nihonium")) {
            if (presetCallback_) (*presetCallback_)(ModelPresets::nihoniumParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Nihonium emu.");

        if (ImGui::Button("Gabriel's P138 Oscillator")) {
            if (presetCallback_) (*presetCallback_)(ModelPresets::gabrielsPOneThirtyEightParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cool period 138 oscillator discovered by Gabriel Nivasch on October 13, 2002.");
	}

    ImGui::End();
    ImGui::Render();
}


Interface::~Interface() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}