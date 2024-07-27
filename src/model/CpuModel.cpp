#include "CpuModel.hpp"
#include "presets/modelpresets.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SDL.h>

void CpuModel::initialize()
{
    generateModel_(activeModelParams_);
}

void CpuModel::resizeGrid_()
{
	grid_.resize(activeModelParams_.modelHeight, std::vector<uint8_t>(activeModelParams_.modelWidth, 0));
    for (auto& row : grid_) {
		row.resize(activeModelParams_.modelWidth, 0);
	}
}

void CpuModel::clearGrid_()
{
    for (auto& row : grid_) {
        row.assign(row.size(), 0);
	}
}

void CpuModel::setParameters(const ModelParameters& modelParameters)
{
	activeModelParams_ = modelParameters;
}

ModelParameters CpuModel::getParameters()
{
	return activeModelParams_;
}

//Should I subclass the datastructure?
//Right now I have a vector of vectors,
//Next I would do a sparse matrix.
//I would reuse most of the update method (aside from the neighbor counting). 
//I could have a countNeighbors(cell). 
//For a sparse matrix, how do I know which dead cells to check? 
//I could keep a list of candidates.. so if the central cell is alive at the end of update, add all of the neighbors to a list of cells to check next time. 

void CpuModel::update()
{
    std::vector<std::vector<uint8_t>> previousState = grid_;
    int livingNeighbors = 0;
    bool cellAlive = false;
    int rowCount = grid_.size();
    int columnCount = grid_[0].size();
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            uint8_t* cellValue = &grid_[rowIndex][columnIndex];
            cellAlive = (*cellValue == aliveValue_) ? true: false;
            livingNeighbors = 0;
            //count living neighbors

            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                int neighborRowIndex = rowIndex + neighborRow;

                //wrap the rows
                if (neighborRowIndex < 0) neighborRowIndex = rowCount - 1;
                if (neighborRowIndex >= rowCount) neighborRowIndex = 0;

                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //skip center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;

                    int neighborColumnIndex = columnIndex + neighborColumn;

                    //wrap the columnsa
                    if (neighborColumnIndex < 0) neighborColumnIndex = columnCount - 1;
                    if (neighborColumnIndex >= columnCount) neighborColumnIndex = 0;

                    //count
                    if (previousState[neighborRowIndex][neighborColumnIndex] == aliveValue_) livingNeighbors++;
                }
            }

            //If not alive and has 3 neighbors, become alive
            if (!cellAlive) {
                if (livingNeighbors == activeModelParams_.rule4) *cellValue = aliveValue_;
                else *cellValue = (*cellValue >= deadValueDecrement_) ? *cellValue -= deadValueDecrement_ : 0;
                //else grid_[rowIndex][columnIndex] -= deadValueDecrement_;
            }
            //If neighbors are less than 2 or more than 3, kill it.
            else if (livingNeighbors < activeModelParams_.rule1 || livingNeighbors > activeModelParams_.rule3) {
                *cellValue = (*cellValue >= deadValueDecrement_) ? *cellValue -= deadValueDecrement_ : 0;
            }
            //if (grid_[rowIndex][columnIndex] < 0) grid_[rowIndex][columnIndex] = 0;
        }
    }
}

void CpuModel::draw(SDL_Renderer* renderer, const int posX, const int posY, const int width, const int height)
{
    //TODO: Currently I am redrawing the model every time. It might be better to draw it to a texture and reuse the texture if model is not updated.
    //I might even update the texture at the same time I update the model, if the view has drawn the last one. 
    //THe hard thing there is making sure that we aren't drawing the texture more often than we need. 
    //I could have a bool that says the texture needs updating before drawing and handle that in core.cpp.

    const CpuModel::GridDrawRange drawRange = getDrawRange_(width, height);
    //drawDecay_(renderer, width, height, drawRange);

    //if(drawStrategy_ != DrawStrategy::DualColor) return;
    for (int rowIndex = drawRange.rowBegin; rowIndex <= drawRange.rowEnd; rowIndex++)
    {
        for (int columnIndex = drawRange.columnBegin; columnIndex <= drawRange.columnEnd; columnIndex++)
        {
            SDL_Color color = SDL_Color();
            if (drawStrategy_ == DrawStrategy::DualColor) {
                auto floatColor = (grid_[rowIndex][columnIndex] == aliveValue_) ? dualColorAliveColor_ : dualColorDeadColor_;

                color = { 
                    (uint8_t)(floatColor[0] * 255), 
                    (uint8_t)(floatColor[1] * 255),
                    (uint8_t)(floatColor[2] * 255),
                    255 
                };
            }
            else {
				color = colormapLookup_[grid_[rowIndex][columnIndex]];
			}

            SDL_SetRenderDrawColor(
                renderer,
                color.r,
                color.g,
                color.b,
                255);

            SDL_Rect rect =
            {
                activeModelParams_.zoomLevel * columnIndex + activeModelParams_.displacementX,
                activeModelParams_.zoomLevel * rowIndex + activeModelParams_.displacementY,
                activeModelParams_.zoomLevel,
                activeModelParams_.zoomLevel
            };

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void CpuModel::drawImGuiWidgets(const bool& isModelRunning)
{
    ImGuiInputTextFlags modelRunningFlag = isModelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

    ImGui::SliderInt(
        "X displacement",
        &activeModelParams_.displacementX,
        -500,
        500);
    ImGui::SliderInt(
        "Y displacement",
        &activeModelParams_.displacementY,
        -500,
        500);

    if (ImGui::CollapsingHeader("CPU Model Parameters")) {

        //TODO:: limit to positive values
        //TODO:: ensure that input is 4-byte aligned
        ImGui::InputInt("Width", &activeModelParams_.modelWidth, 100, 100, modelRunningFlag);
        ImGui::InputInt("Height", &activeModelParams_.modelHeight, 100, 100, modelRunningFlag);
        ImGui::SliderFloat("Model Fill Factor", &activeModelParams_.fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        if (ImGui::Button("Generate Model")) {
            generateModel_(activeModelParams_);
        }

        ImGuiInputTextFlags ruleInputFlags = isModelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

        if (ImGui::InputInt("Conway Rule 1 Cutoff", &activeModelParams_.rule1, 1, 1))
        {
            if (activeModelParams_.rule1 < 0) activeModelParams_.rule1 = 0;
            if (activeModelParams_.rule1 > 8) activeModelParams_.rule1 = 8;
            if (activeModelParams_.rule1 >= activeModelParams_.rule3) activeModelParams_.rule1 = activeModelParams_.rule3 - 1;
        };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has fewer than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 3 Cutoff", &activeModelParams_.rule3, 1, 1))
        {
            if (activeModelParams_.rule3 < 1) activeModelParams_.rule3 = 1;
            if (activeModelParams_.rule3 > 8) activeModelParams_.rule3 = 8;
            if (activeModelParams_.rule3 <= activeModelParams_.rule1) activeModelParams_.rule3 = activeModelParams_.rule1 + 1;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a living cell has more than this many neighbors, it dies.");

        if (ImGui::InputInt("Conway Rule 4", &activeModelParams_.rule4, 1, 1))
        {
            if (activeModelParams_.rule4 < 0) activeModelParams_.rule4 = 0;
            if (activeModelParams_.rule4 > 8) activeModelParams_.rule4 = 8;
        };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("If a dead cell has exactly this many neighbors, it becomes alive.");

    }

    //Visualization
    if (ImGui::CollapsingHeader("Visualization")) {
        //Need a button to choose drawing strategy
        
        if (ImGui::Combo("Coloring Strategy", &selectedColorMapIndex_, colorMapSelectorItems_,15))
        {
            drawStrategy_ = (selectedColorMapIndex_ == 0) ? DrawStrategy::DualColor : DrawStrategy::Decay;

            switch (selectedColorMapIndex_){
            case 0:
                break;
            case 1:
                colormapLookup_ = Colormaps::CividisLookup;
                break;
            case 2:
				colormapLookup_ = Colormaps::CubehelixLookup;
				break;
            case 3:
                colormapLookup_ = Colormaps::GithubLookup;
                break;
            case 4:
				colormapLookup_ = Colormaps::GrayLookup;
				break;
            case 5:
                colormapLookup_ = Colormaps::HeatLookup;
                break;
            case 6:
                colormapLookup_ = Colormaps::HotLookup;
                break;
            case 7:
                colormapLookup_ = Colormaps::HSVLookup;
                break;
            case 8:
				colormapLookup_ = Colormaps::InfernoLookup;
				break;
                case 9:
				colormapLookup_ = Colormaps::JetLookup;
				break;
                case 10:
				colormapLookup_ = Colormaps::MagmaLookup;
				break;
                case 11:
                    colormapLookup_ = Colormaps::ParulaLookup;
                    break;
                case 12:
                    colormapLookup_ = Colormaps::PlasmaLookup;
                    break;
                case 13:
					colormapLookup_ = Colormaps::TurboLookup;
					break;
				case 14:
					colormapLookup_ = Colormaps::ViridisLookup;
					break;
            }
        }
        
        if (drawStrategy_ == DrawStrategy::DualColor)
        {
            ImGui::ColorPicker3("Alive Cell Color:", dualColorAliveColor_, 134217728);
            ImGui::ColorPicker3("Dead Cell Color:", dualColorDeadColor_, 134217728);
        }
		else {
			ImGui::InputInt("Decay Rate", &deadValueDecrement_, 1, 10, 0);
            if (deadValueDecrement_ < 1) deadValueDecrement_ = 1;
            else if (deadValueDecrement_ > 255) deadValueDecrement_ = 255;
		}
    }

    if (ImGui::CollapsingHeader("presets"))
    {
        if (ImGui::Button("random")) {
            generateModel_(ModelPresets::randomParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("a randomly generated field to observe conway's game of life.");

        if (ImGui::Button("From File")) {
            auto fileDialog = pfd::open_file(
                "Choose file", 
                pfd::path::home(),
                {"RLE Pattern Files (.rle)", "*.rle"},
                false
            );
            auto result = fileDialog.result();
            if (!result.empty()) {
                std::cout << "Opening: " << fileDialog.result()[0] << "\n";
                std::ifstream filestream(result[0]);

                if (filestream.is_open())
                {
                    clearGrid_();
                    populateFromRLE_(filestream);
                    filestream.close();
                }
            }
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("RLE encoded files can be downloaded from https://conwaylife.com/");

        if (ImGui::Button("From String")) ImGui::OpenPopup("Enter RLE string:");
        if (ImGui::BeginPopup("Enter RLE string:"))
        {
            //I need a temporary input buffer as userInput will be empty once Finished is pressed
            std::string userInput;
            if (ImGui::InputTextMultiline("Enter RLE String:", &userInput))
            {
                inputString_ = userInput;
            }

            if (ImGui::Button("Finished"))
            {
                ImGui::CloseCurrentPopup();
                if (!inputString_.empty())
                {
                    std::cout << "Processing: " << inputString_ << "\n";
                    clearGrid_();
                    std::stringstream rleStream(inputString_);
                    populateFromRLE_(rleStream);
                }
            }
            ImGui::EndPopup();
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("RLE text can be copied and pasted from https://conwaylife.com/");

        if (ImGui::Button("swiss cheese")) {
            generateModel_(ModelPresets::swissCheeseParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("modified rules can produce different results.");

        if (ImGui::Button("decomposition")) {
            generateModel_(ModelPresets::decompositionParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("modified rules can produce different results.");

        if (ImGui::Button("blinker")) {
            generateModel_(ModelPresets::blinkerParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("the smallest oscillator in conway's game of life.");

        if (ImGui::Button("lightweight spaceship")) {
            generateModel_(ModelPresets::lightweightSpaceshipParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("the smallest orthoganal spaceship in conway's game of life.");

        if (ImGui::Button("blocker")) {
            generateModel_(ModelPresets::blockerParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("blocker.");

        if (ImGui::Button("nihonium")) {
            generateModel_(ModelPresets::nihoniumParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("nihonium emu.");

        if (ImGui::Button("gabriel's p138 oscillator")) {
            generateModel_(ModelPresets::gabrielsPOneThirtyEightParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("cool period 138 oscillator discovered by gabriel nivasch on october 13, 2002.");
    }
}

void CpuModel::handleSDLEvent(const SDL_Event& event)
{
    int mousePosX, mousePosY;
    int mouseButtonState = SDL_GetMouseState(&mousePosX, &mousePosY);
    //ImGui::IsWindowHovered(4) returns true if any window is hovered. If you pass 0, it throws an exception as it does not know the window to check.
    //std::cout << ImGui::IsWindowHovered(4);
    
    if (!ImGui::IsWindowHovered(4) && !ImGui::IsAnyItemActive())
    {
        if (mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) && event.type == SDL_MOUSEMOTION)
        {
            activeModelParams_.displacementX += event.motion.xrel;
            activeModelParams_.displacementY += event.motion.yrel;
            //TODO:Check that it is within bounds of a maximum displacement

        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            int width, height;
            SDL_GetWindowSize(SDL_GetWindowFromID(event.window.windowID), &width, &height);
            //Zoom
            //TODO:: only allow scales that are a multiple of pixels.
            double newZoomLevel = activeModelParams_.zoomLevel;
            if (event.wheel.y > 0)
            {
                //newZoomLevel = (activeModelParams_.zoomLevel * 1.2);
                newZoomLevel = activeModelParams_.zoomLevel + 1;
                if (newZoomLevel > CpuModel::MAX_ZOOM) newZoomLevel = CpuModel::MAX_ZOOM;
            }
            else if (event.wheel.y < 0) {
                //newZoomLevel = activeModelParams_.zoomLevel * 0.8;
                newZoomLevel = activeModelParams_.zoomLevel - 1;
                if (newZoomLevel < CpuModel::MIN_ZOOM) newZoomLevel = CpuModel::MIN_ZOOM;
            }
            
            //map the cursor position in viewport space to the index in model space. Zoomlevel is the width of 1 cell in viewport space.
            int cursorModelIndexX = (int)(((double)mousePosX - (double)activeModelParams_.displacementX) / activeModelParams_.zoomLevel);
            int cursorModelIndexY = (int)(((double)mousePosY - (double)activeModelParams_.displacementY) / activeModelParams_.zoomLevel);
            activeModelParams_.zoomLevel = newZoomLevel;
            //Calculate the new displacement, trying to keep the model in the same position relative to the cursor. 
            //This works MOSTLY but drifts more than I would like. 
            activeModelParams_.displacementX = (int)(-((double)cursorModelIndexX * newZoomLevel) + (double)mousePosX);
            activeModelParams_.displacementY = (int)(-((double)cursorModelIndexY * newZoomLevel) + (double)mousePosY);

            
        }
    }
}

void CpuModel::generateModel_(const ModelParameters& params) {
    //First set the members to correspond with the parameters
    if (activeModelParams_.modelWidth < params.minWidth) activeModelParams_.minWidth = params.minWidth;
    if (activeModelParams_.modelHeight < params.minHeight) activeModelParams_.minHeight = params.minHeight;
    if (params.modelWidth > 0) activeModelParams_.modelWidth = params.modelWidth;
    if (params.modelHeight > 0) activeModelParams_.modelHeight = params.modelHeight;
    if (params.fillFactor > 0) activeModelParams_.fillFactor = params.fillFactor;
    if (params.rule1 > 0) activeModelParams_.rule1 = params.rule1;
    if (params.rule3 > 0) activeModelParams_.rule3 = params.rule3;
    if (params.rule4 > 0) activeModelParams_.rule4 = params.rule4;

    if (grid_.size() != activeModelParams_.modelHeight || grid_[0].size() != activeModelParams_.modelWidth) {
		resizeGrid_();
	}
    else {
		clearGrid_();
	}

    if (params.random) {
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (auto& row : grid_) {
            for (auto& cell : row) {
				cell = distribution(rng) < params.fillFactor ? aliveValue_ : deadValue_;
			}
		}
        std::cout << "Random model generated" << std::endl;
        return;
    }

    else {
        if (!params.runLengthEncoding.empty()) {
            std::istringstream rleStream(params.runLengthEncoding);
            populateFromRLE_(rleStream);
        }
    }
}

void CpuModel::populateFromRLE_(std::istream& modelStream)
{
    std::string line = "";
    std::string RLEstring = "";
    while(std::getline(modelStream, line))
    {
        //Find the header line containing specifications, which begins with the char 'x'
		if (line[0] == '#') continue;
		if (line[0] == 'x') {
            std::string::iterator lineIterator = line.begin();
            std::string minWidthString = "";
            std::string minHeightString = "";
            int bornNeighborCount = 3;
            int surviveNeighborMin = 2;
            int surviveNeighborMax = 3;

            //minimum width
            while (!std::isdigit(*lineIterator)) lineIterator++;
            while (std::isdigit(*lineIterator)) {
				minWidthString += *lineIterator;
				lineIterator++;
			}
            activeModelParams_.minWidth = std::stoi(minWidthString);

            //minimum height
            while (!std::isdigit(*lineIterator)) lineIterator++;
            while (std::isdigit(*lineIterator)) {
                minHeightString += *lineIterator;
                lineIterator++;
            }
            activeModelParams_.minHeight = std::stoi(minHeightString);
            
            //Neighbor count to be born
            while (*lineIterator != 'B' && *lineIterator != 'b') lineIterator++;
            lineIterator++;
            activeModelParams_.rule4 = (int)(*lineIterator - '0');
            lineIterator++;
            //minimum and maximum neighbors to survive
            while (!std::isdigit(*lineIterator)) lineIterator++;
            activeModelParams_.rule1 = (int)(*lineIterator - '0');
            lineIterator++;
            activeModelParams_.rule3 = (int)(*lineIterator - '0');//lineiterator '/S23'
            continue;
		}

        //If lines don't start with # or X, they must be part of the RLE encoded model.
        RLEstring += line;
	}
    activeModelParams_.modelWidth = std::max<int>(activeModelParams_.modelWidth, activeModelParams_.minWidth);
    activeModelParams_.modelHeight= std::max<int>(activeModelParams_.modelHeight, activeModelParams_.minHeight);
    if (grid_.size() != activeModelParams_.modelHeight || grid_[0].size() != activeModelParams_.modelWidth) {
        resizeGrid_();
    }
    else {
        clearGrid_();
    }

    int startColumn = (activeModelParams_.modelWidth / 2) - (activeModelParams_.minWidth / 2);
    int startRow = activeModelParams_.modelHeight / 2 - (activeModelParams_.minHeight / 2);
    int row = startRow;
    int column = startColumn;

    std::string::iterator RLEit;
    for (std::string::iterator it = RLEstring.begin(); it != RLEstring.end(); ++it)
    {
        if (*it == '!') break;

        std::string stringInteger = "";
        while ( isdigit(*it) || *it == '\n')
        {
            if (*it != '\n') stringInteger += *it;
            it++;
        }
        //If there is no preceding integer, set the count to 1
        if (stringInteger.empty()) stringInteger = "1";
        int count = std::stoi(stringInteger);
        //b is dead, o is alive, $ is newline
        if (*it == 'b') {
            for (int i = 0; i < count; i++) {
                column++;
            }
        }
        else if (*it == 'o') {
            for (int i = 0; i < count; i++) {
                grid_[row][column] = aliveValue_;
                column++;
            }
        }
        else if (*it == '$')
        {
            column = startColumn;
            for (int i = 0; i < count; i++) {
                row++;
            }
        }
    }
}

CpuModel::GridDrawRange CpuModel::getDrawRange_(int width, int height)
{
    CpuModel::GridDrawRange drawRange;
    drawRange.rowBegin = 0-(int)(((double)activeModelParams_.displacementY / activeModelParams_.zoomLevel));
    drawRange.rowEnd = drawRange.rowBegin + (height / activeModelParams_.zoomLevel);
    drawRange.columnBegin = -(int)(((double)activeModelParams_.displacementX / activeModelParams_.zoomLevel));
    /*drawRange.columnEnd = drawRange.columnBegin + (columnCount / activeModelParams_.zoomLevel) - 1;*/
    drawRange.columnEnd = drawRange.columnBegin + (width / activeModelParams_.zoomLevel);

    //make sure you don't try and draw something not in grid_
    int gridRows = grid_.size();
    int gridColumns = grid_[0].size();
    if (drawRange.rowEnd >= gridRows) drawRange.rowEnd = gridRows - 1;
    if (drawRange.columnEnd >= gridColumns) drawRange.columnEnd = gridColumns - 1;
    if (drawRange.rowBegin < 0) drawRange.rowBegin = 0;
    if (drawRange.columnBegin < 0) drawRange.columnBegin = 0;

    return drawRange;
}