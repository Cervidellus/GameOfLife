#include <gui\interface.hpp>
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
    std::function<void(ModelParameters presetParameters)> presetCallback, 
    std::function<SDL_Texture* ()> getWindowTextureCallback)
{
    presetCallback_ = std::make_unique<std::function<void(ModelParameters presetParameters)>>(std::move(presetCallback));
    getWindowTextureCallback_ = std::make_unique<std::function<SDL_Texture*()>>(std::move(getWindowTextureCallback));

    isInitialized_ = true;
	return true;
}

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

void Interface::draw(
    SDL_Renderer* renderer,
    bool& modelRunning,
    int& desiredModelFPS,
    ModelParameters& modelParams,
    const int measuredModelFPS
)
{
    //IMGUI
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

    ImGuiInputTextFlags modelRunningFlag = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;
    
    if (ImGui::CollapsingHeader("Parameters")) {

        //TODO:: limit to positive values
        //TODO:: ensure that input is 4-byte aligned
        ImGui::InputInt("Width", &modelParams.modelWidth, 100, 100, modelRunningFlag);
        ImGui::InputInt("Height", &modelParams.modelHeight, 100, 100, modelRunningFlag);

        ImGui::SliderFloat("Model Fill Factor", &modelParams.fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        //if (ImGui::Button("Generate Model")) {
        //    if(generateModelCallback_) (*generateModelCallback_)(ModelPreset::random);
        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Start Model")) {
        //    modelRunning = true;
        //}

        //ImGuiInputTextFlags ruleInputFlags = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

        if (ImGui::InputInt("Conway Rule 1 Cutoff", &modelParams.rule1, 1, 1))
        {
            if (modelParams.rule1 < 0) modelParams.rule1 = 0;
            if (modelParams.rule1 > 8) modelParams.rule1 = 8;
            if (modelParams.rule1 >= modelParams.rule3) modelParams.rule1 = modelParams.rule3 - 1;
        };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has fewer than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 3 Cutoff", &modelParams.rule3, 1, 1))
        {
            if (modelParams.rule3 < 1) modelParams.rule3 = 1;
            if (modelParams.rule3 > 8) modelParams.rule3 = 8;
            if (modelParams.rule3 <= modelParams.rule1) modelParams.rule3 = modelParams.rule1 + 1;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has more than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 4", &modelParams.rule4, 1, 1))
        {
            if (modelParams.rule4 < 0) modelParams.rule4 = 0;
            if (modelParams.rule4 > 8) modelParams.rule4 = 8;
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

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}


Interface::~Interface() {
    if (isInitialized_) {
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}