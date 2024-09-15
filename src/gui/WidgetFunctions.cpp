#include "WidgetFunctions.hpp"
#include "../presets/modelpresets.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SDL3/SDL.h>

void WidgetFunctions::drawGOLRulesHeader(
    ModelParameters& modelParameters,
    std::function<void(const ModelParameters&)> generateModelCallback,
    const bool modelRunning)
{
    ImGuiInputTextFlags modelRunningFlag = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;
    if (ImGui::CollapsingHeader("Game Of Life Parameters")) {

        //TODO:: limit to positive values
        //TODO:: ensure that input is 4-byte aligned
        ImGui::InputInt("Width", &modelParameters.modelWidth, 100, 100, modelRunningFlag);
        ImGui::InputInt("Height", &modelParameters.modelHeight, 100, 100, modelRunningFlag);
        ImGui::SliderFloat("Model Fill Factor", &modelParameters.fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        if (ImGui::Button("Generate Model")) {
            generateModelCallback(modelParameters);
        }

        if (ImGui::InputInt("Conway Rule 1 Cutoff", &modelParameters.rule1, 1, 1))
        {
            if (modelParameters.rule1 < 0) modelParameters.rule1 = 0;
            if (modelParameters.rule1 > 8) modelParameters.rule1 = 8;
            if (modelParameters.rule1 >= modelParameters.rule3) modelParameters.rule1 = modelParameters.rule3 - 1;
        };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has fewer than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 3 Cutoff", &modelParameters.rule3, 1, 1))
        {
            if (modelParameters.rule3 < 1) modelParameters.rule3 = 1;
            if (modelParameters.rule3 > 8) modelParameters.rule3 = 8;
            if (modelParameters.rule3 <= modelParameters.rule1) modelParameters.rule3 = modelParameters.rule1 + 1;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has more than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 4", &modelParameters.rule4, 1, 1))
        {
            if (modelParameters.rule4 < 0) modelParameters.rule4 = 0;
            if (modelParameters.rule4 > 8) modelParameters.rule4 = 8;
        };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a dead cell has exactly this many neighbors, it becomes alive.");

    }
}

void WidgetFunctions::drawVisualizationHeader(
	ModelParameters& modelParameters,
	ColorMapper& colorMapper,
    int& colorDecayRate,
    bool& displacementChanged,
	const bool modelRunning)
{
    if (ImGui::CollapsingHeader("Visualization")) {

        if(ImGui::SliderInt("Zoom Level", &modelParameters.zoomLevel, 1, 100)) displacementChanged = true;

        if(ImGui::SliderInt(
            "X displacement",
            &modelParameters.displacementX,
            -500,
            500)) displacementChanged = true;
        if (ImGui::SliderInt(
            "Y displacement",
            &modelParameters.displacementY,
            -500,
            500))displacementChanged = true;
        if(ImGui::Button("Recenter"))
		{
			modelParameters.displacementX = 0;
			modelParameters.displacementY = 0;
            //I need to retrigger a recalcDrawRange from this?
            displacementChanged = true;
		}

        ImGui::Combo("Coloring Strategy", &colorMapper.selectedColorMapIndex, colorMapper.ColorMapNames, 15);

        if (colorMapper.selectedColorMapIndex == (int)ColorMapper::ColormapType::DualColor)
        {
            auto floatAliveColor= colorMapper.getDualColorAliveColorFloat().data();
            auto floatDeadColor = colorMapper.getDualColorDeadColorFloat().data();
            if (ImGui::ColorPicker3 ("Alive Cell Color:", floatAliveColor, 134217728))
            {
                colorMapper.setDualColorAliveColor(
                    *reinterpret_cast<std::array<float, 3>*>(floatAliveColor)
                );
            }
            if (ImGui::ColorPicker3("Dead Cell Color:", floatDeadColor, 134217728))
            {
                colorMapper.setDualColorDeadColor(
                    *reinterpret_cast<std::array<float, 3>*>(floatDeadColor)
                );
            }
        }
        else {
            ImGui::InputInt("Decay Rate", &colorDecayRate, 1, 10, 0);
            colorDecayRate = std::clamp<int>(colorDecayRate, 1, 255);
        }
    }

}

void WidgetFunctions::drawPresetsHeader(
    ModelParameters& modelParameters,
    std::function<void(const ModelParameters&)> generateModelCallback,
    std::function<void(std::string)> loadPresetFileCallback,
    std::function<void()> loadRLEStringCallback,
    std::string& RLEString,
    const bool modelRunning
)
{
    if (ImGui::CollapsingHeader("Presets"))
    {
        if (ImGui::Button("random")) {
            generateModelCallback(ModelPresets::randomParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("a randomly generated field to observe conway's game of life.");

        //Here I should just return result in a callback, and handle the file in the other object.
        if (ImGui::Button("From File")) {
            auto fileDialog = pfd::open_file(
                "Choose file",
                pfd::path::home(),
                { "RLE Pattern Files (.rle)", "*.rle" },
                false);
            auto result = fileDialog.result();
            if (!result.empty()) loadPresetFileCallback(result[0]);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("RLE encoded files can be downloaded from https://conwaylife.com/");

        if (ImGui::Button("From String")) ImGui::OpenPopup("Enter RLE string:");
        if (ImGui::BeginPopup("Enter RLE string:"))
        {
            ImGui::InputTextMultiline("Enter RLE String:", &RLEString);

            if (ImGui::Button("Finished"))
            {
                ImGui::CloseCurrentPopup();
                if (!RLEString.empty()) loadRLEStringCallback();
            }
            ImGui::EndPopup();
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("RLE text can be copied and pasted from https://conwaylife.com/");

        if (ImGui::Button("swiss cheese")) {
            generateModelCallback(ModelPresets::swissCheeseParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("modified rules can produce different results.");

        if (ImGui::Button("decomposition")) {
            generateModelCallback(ModelPresets::decompositionParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("modified rules can produce different results.");

        //if (ImGui::Button("blinker")) {
        //    generateModelCallback(ModelPresets::blinkerParams);
        //}
        //if (ImGui::IsItemHovered()) ImGui::SetTooltip("the smallest oscillator in conway's game of life.");

        //if (ImGui::Button("lightweight spaceship")) {
        //    generateModelCallback(ModelPresets::lightweightSpaceshipParams);
        //}
        //if (ImGui::IsItemHovered()) ImGui::SetTooltip("the smallest orthoganal spaceship in conway's game of life.");

        //if (ImGui::Button("blocker")) {
        //    generateModelCallback(ModelPresets::blockerParams);
        //}
        //if (ImGui::IsItemHovered()) ImGui::SetTooltip("blocker.");

        if (ImGui::Button("nihonium")) {
            generateModelCallback(ModelPresets::nihoniumParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("nihonium emu.");



        if (ImGui::Button("gabriel's p138 oscillator")) {
            generateModelCallback(ModelPresets::gabrielsPOneThirtyEightParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("cool period 138 oscillator discovered by gabriel nivasch on october 13, 2002.");

        if (ImGui::Button("Backrack 1 Puffer")) {
            generateModelCallback(ModelPresets::backrakeOnePufferVarTwo);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Variation 2 of backrake puffer discovered by Jason Summers in 2001.");

        if (ImGui::Button("Connected Phoenix Parts")) {
            generateModelCallback(ModelPresets::connectedPhoenix);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("In a phoenix pattern, all live cells die every generation. Here a are a bunch of them connected together.");

        if (ImGui::Button("Double X")) {
            generateModelCallback(ModelPresets::doubleX);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 46 oscillator discovered by Robert Wainwright");

        if (ImGui::Button("Frothing Puffer Rake")) {
            generateModelCallback(ModelPresets::frothingPufferRake);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 270 puffer rake.");

        if (ImGui::Button("New Shuttle")) {
            generateModelCallback(ModelPresets::newShuttle);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 28 oscillator discovered by David Buckingham in 1973.");

        if (ImGui::Button("P28 Glider Shuttle")) {
            generateModelCallback(ModelPresets::pTwentyEightGliderShuttle);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 28 glider shuttle constructed from 4 34P14 glider shuttles. Discovered 2018.");

        if (ImGui::Button("p47 B-heptomino hassler")) {
            generateModelCallback(ModelPresets::pFourtySevenBHeptominoHassler);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Constructed from P47 Pre-Pulsar Shuttles.");

        if (ImGui::Button("Pre-Pulsar Shuttle 26")) {
            generateModelCallback(ModelPresets::prePulsarShuttle26);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 26 Pre-Pulsar Shuttle Oscillatory discovered by David Buckingham in 1983.");

        if (ImGui::Button("Ring Of Fire")) {
            generateModelCallback(ModelPresets::ringOfFire);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Period 2 oscillator discovered by Dean Hickerson in 1992.");

        if (ImGui::Button("Sir Robin")) {
            generateModelCallback(ModelPresets::sirRobbin);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Interesting spaceship that known as a knightship that travels like a chess knight rather than on a pure diagonal.");

        if (ImGui::Button("Slow Puffer 2")) {
            generateModelCallback(ModelPresets::slowPuffer2);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Puffer that forms a large wick.");

        if (ImGui::Button("T-Nosed P8")) {
            generateModelCallback(ModelPresets::tNosedP8);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("T-Nosed P8.");

        if (ImGui::Button("Vex Stabilization")) {
            generateModelCallback(ModelPresets::vexStabilisation);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stabilization of Vex, a pattern discovered by Achim Flammenkamp in 1994.");
    }
}