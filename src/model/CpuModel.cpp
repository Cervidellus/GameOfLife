#include "CpuModel.hpp"
#include "presets/modelpresets.hpp"
#include "gui/sdl_colormaps_8bit.hpp"

#include <iostream>
#include <random>

#include <imgui.h>
#include <SDL.h>


//Draw Strategies:
//-Single Color
//Decay: Color decays according to a colormap according to how long the pixel has been dead. 
//Something based on hte values of neighbors? Decay slower when you have more neighbors?
//Color based on average value of neighbors?


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
    const CpuModel::GridDrawRange drawRange = getDrawRange_(width, height);
    drawDecay_(renderer, width, height, drawRange);

 //   SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_SetRenderDrawColor(renderer, (uint8_t)(singleDrawColor_[0] * 255), (uint8_t)(singleDrawColor_[1] * 255), (singleDrawColor_[2] * 255), 255);

 //   for(int rowIndex = drawRange.rowBegin; rowIndex <= drawRange.rowEnd; rowIndex++)
	//{
	//	for(int columnIndex = drawRange.columnBegin; columnIndex <= drawRange.columnEnd; columnIndex++)
	//	{
	//		if(grid_[rowIndex][columnIndex] == aliveValue_)
	//		{
 //               //I might be able to add the remainder of the displacement to get smoother panning...
 //               SDL_Rect rect = 
 //               { 
 //                   activeModelParams_.zoomLevel * (columnIndex - drawRange.columnBegin),
 //                   activeModelParams_.zoomLevel * (rowIndex - drawRange.rowBegin),
 //                   activeModelParams_.zoomLevel,
 //                   activeModelParams_.zoomLevel
 //               };
 //               SDL_RenderFillRect(renderer, &rect);
	//		}
	//	}
	//}

}

void CpuModel::drawDecay_(SDL_Renderer* renderer, const int width, const int height, const GridDrawRange& drawRange) {
    for (int rowIndex = drawRange.rowBegin; rowIndex <= drawRange.rowEnd; rowIndex++)
    {
        for (int columnIndex = drawRange.columnBegin; columnIndex <= drawRange.columnEnd; columnIndex++)
        {
            //Set the color according to the value.
            //I might be able to add the remainder of the displacement to get smoother panning...

            //I guess this part is all that needs to be separate from other strategies.
            SDL_Rect rect =
            {

                activeModelParams_.zoomLevel * columnIndex + activeModelParams_.displacementX /*+ (activeModelParams_.displacementX % width)*/,
                activeModelParams_.zoomLevel * rowIndex + activeModelParams_.displacementY /*- (activeModelParams_.displacementY % height)*/,
                activeModelParams_.zoomLevel,
                activeModelParams_.zoomLevel
            };
            //This is actually quite slow.
            //TODO::Switch to a lookup table.

            
            auto color = tinycolormap::GetQuantizedColor(
                (double)grid_[rowIndex][columnIndex] / 255.0,
                100,
                colorMapType_);
            SDL_SetRenderDrawColor(renderer, color.ri(), color.gi(), color.bi(), 255);

            //const auto color = Colormaps::ViridisLookup[grid_[rowIndex][columnIndex]];
            //SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            //if ((double)grid_[rowIndex][columnIndex] == aliveValue_) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //else SDL_SetRenderDrawColor(renderer,  0, 0, 0, 255);
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
        ImGui::ColorPicker3("RGB Color:", singleDrawColor_, 134217728);

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

    if (ImGui::CollapsingHeader("presets"))
    {
        if (ImGui::Button("random")) {
            generateModel_(ModelPresets::randomParams);
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("a randomly generated field to observe conway's game of life.");

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
    }
    else {
        //if incoded in a std::vector<std::pair<int, int>> we want it centered.
        const int startColumn = (activeModelParams_.modelWidth / 2) - (params.minWidth / 2);
        const int startRow = activeModelParams_.modelHeight / 2 - (params.minHeight / 2);
        if (params.aliveCells.size() > 0) {
            for (const std::pair<int,int> aliveCell : params.aliveCells) {
                if (aliveCell.first < 0 || aliveCell.first >= params.minHeight) continue;
                grid_[startRow + aliveCell.first][startColumn + aliveCell.second] = 255;
            }
        }

        //RLE encoding
        if (!params.runLengthEncoding.empty()) {
            populateFromRLEString_(
                params.runLengthEncoding,
                startRow,
                startColumn
            );
        }
    }
}

void CpuModel::populateFromRLEString_(
    std::string model,
    const int startRow,
    const int startColumn)
{
    int row = startRow;
    int column = startColumn;

    for (std::string::iterator modelIterator = model.begin(); modelIterator != model.end(); modelIterator++) {
        //First handle the edge cases

        if (*modelIterator == '!') break;

        int count = 1;
        if (isdigit(*modelIterator))
        {
            std::string stringInteger = "";
            while (isdigit(*modelIterator) && modelIterator != model.end())
            {
                stringInteger += *modelIterator;
                modelIterator++;
            }
            count = std::stoi(stringInteger);
        }
        //b is dead, o is alive, $ is newline
        if (*modelIterator == 'b') {
            for (int i = 0; i < count; i++) {
                column++;
            }
        }
        else if (*modelIterator == 'o') {
            for (int i = 0; i < count; i++) {
                grid_[row][column] = aliveValue_;
                column++;
            }
        }
        else if (*modelIterator == '$')
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