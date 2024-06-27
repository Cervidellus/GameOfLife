#include "CpuModel.hpp"

#include <iostream>
#include <random>

//#include <imgui_impl_sdl2.h>
//#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>
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

void CpuModel::update()
{
    std::vector<std::vector<uint8_t>> previousState = grid_;
    int livingNeighbors = 0;
    bool cellAlive = false;
    int rowCount = grid_.size();
    int columnCount = grid_[0].size();
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            cellAlive = (grid_[rowIndex][columnIndex] == aliveValue_) ? aliveValue_ : deadValue_;
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

                    //neighborColumn = columnIndex + neighborColumn;

                    //wrap the columns
                    if (neighborColumnIndex < 0) neighborColumnIndex = columnCount - 1;
                    if (neighborColumnIndex >= columnCount) neighborColumnIndex = 0;

                    //count
                    if (previousState[neighborRowIndex][neighborColumnIndex] == aliveValue_) livingNeighbors++;
                }
            }

            //If not alive and has 3 neighbors, become alive
            if (!cellAlive) {
                if (livingNeighbors == activeModelParams_.rule4) grid_[rowIndex][columnIndex] = aliveValue_;
            }
            //If neighbors are less than 2 or more than 3, kill it.
            else if (livingNeighbors < activeModelParams_.rule1 || livingNeighbors > activeModelParams_.rule3) grid_[rowIndex][columnIndex] = deadValue_;
        }
    }
}

void CpuModel::draw(SDL_Renderer* renderer, const int posX, const int posY, const int width, const int height)
{
    //to test this quickly, I can just create a surface and draw it to the texture to see if everything else is working.
    //If that works, I'll draw squares for each cell.
    //Once THAT works I can pull it out into a draw strategy so I can do things differently.
    
    //First I can iterate over all and see if it works.(done)
    //Then I can put in place what i need to only draw what is on screen.(done.. mostly)
    //Then I can add the ability to zoom.
    //Then I can add the ability to pan.(done)

    int gridRows = grid_.size();
    int gridColumns = grid_[0].size();
    const float rectPadding = 0.1;
    const float scale = 1.0;
    
    //Figure out number of rows and columns to draw
    int rowsToDrawCount = std::min<int>(gridRows, height);
    int columnsToDrawCount = std::min<int>(gridColumns, width);

    //I need to add a concept of scale here.
    //Figure out the starting indices to draw
    int rowDrawStartIndex = 0;
    int columnDrawStartIndex = 0;
    if(gridRows > height) rowDrawStartIndex = (gridRows - rowsToDrawCount) / 2;
    if(gridColumns > width) columnDrawStartIndex = (gridColumns - columnsToDrawCount) / 2;

    int rowDrawEndIndex = rowDrawStartIndex + rowsToDrawCount - 1;
    int columnDrawEndIndex = columnDrawStartIndex + columnsToDrawCount - 1;

    //Determine rect size
    //const int rectWidth = int(scale * ((float)width / (float)columnsToDrawCount));

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    //This allows for it to be centered when it is smaller than the window.
    int drawStartX = posX + activeModelParams_.displacementX + (width - columnsToDrawCount) / 2;
    int drawStartY = posY + activeModelParams_.displacementY + (height - rowsToDrawCount) / 2;
    //Set to zero if model is larger than window.
    //if(drawStartX < 0) drawStartX = 0;
    //if(drawStartY < 0) drawStartY = 0;
    //I should also check for maximum.

    //Hopefully this iterates just over the cells that are visible.
    //I need to test this.. 
    for(int rowIndex = rowDrawStartIndex; rowIndex <= rowDrawEndIndex; rowIndex++)
	{
		for(int columnIndex = columnDrawStartIndex; columnIndex <= columnDrawEndIndex; columnIndex++)
		{
			if(grid_[rowIndex][columnIndex] == aliveValue_)
			{
                SDL_Rect rect = 
                { 
                    drawStartX + activeModelParams_.zoomLevel * (columnIndex - columnDrawStartIndex), 
                    drawStartY + activeModelParams_.zoomLevel * (rowIndex - rowDrawStartIndex),
                    activeModelParams_.zoomLevel,
                    activeModelParams_.zoomLevel
                };
                SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

void CpuModel::drawImGuiWidgets(const bool isModelRunning)
{
    ImGuiInputTextFlags modelRunningFlag = isModelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

    if (ImGui::CollapsingHeader("CPU Model Parameters")) {

        //TODO:: limit to positive values
        //TODO:: ensure that input is 4-byte aligned
        ImGui::InputInt("Width", &activeModelParams_.modelWidth, 100, 100, modelRunningFlag);
        ImGui::InputInt("Height", &activeModelParams_.modelHeight, 100, 100, modelRunningFlag);

        ImGui::SliderFloat("Model Fill Factor", &activeModelParams_.fillFactor, 0.001, 1);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("The proportion of 'alive' cells generated.");
        //if (ImGui::Button("Generate Model")) {
        //    if(generateModelCallback_) (*generateModelCallback_)(ModelPreset::random);
        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Start Model")) {
        //    modelRunning = true;
        //}

        //ImGuiInputTextFlags ruleInputFlags = modelRunning ? ImGuiInputTextFlags_ReadOnly : 0;

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

    //if (ImGui::CollapsingHeader("Presets"))
    //{
    //    if (ImGui::Button("random")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::randomParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("A randomly generated field to observe Conway's Game of Life.");

    //    if (ImGui::Button("Swiss Cheese")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::swissCheeseParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Modified rules can produce different results.");

    //    if (ImGui::Button("Decomposition")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::decompositionParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Modified rules can produce different results.");

    //    if (ImGui::Button("Blinker")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::blinkerParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("The smallest oscillator in Conway's Game of Life.");

    //    if (ImGui::Button("Lightweight Spaceship")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::lightweightSpaceshipParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("The smallest orthoganal spaceship in Conway's Game of Life.");

    //    if (ImGui::Button("Blocker")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::blockerParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Blocker.");

    //    if (ImGui::Button("Nihonium")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::nihoniumParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Nihonium emu.");

    //    if (ImGui::Button("Gabriel's P138 Oscillator")) {
    //        if (presetCallback_) (*presetCallback_)(ModelPresets::gabrielsPOneThirtyEightParams);
    //    }
    //    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cool period 138 oscillator discovered by Gabriel Nivasch on October 13, 2002.");
    //}
}

void CpuModel::handleSDLEvent(const SDL_Event& event)
{
    int x,y;
    int mouseButtonState = SDL_GetMouseState(&x, &y);
    if (mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) && event.type == SDL_MOUSEMOTION)
    {
        //Change displacement
        activeModelParams_.displacementX += event.motion.xrel;
        activeModelParams_.displacementY += event.motion.yrel;
        //Check that it is within bounds of a maximum displacement

    }
    else if (event.type == SDL_MOUSEWHEEL)
	{
		//Zoom
        //TODO:: only allow scales that are a multiple of pixels.
		if (event.wheel.y > 0) {
			activeModelParams_.zoomLevel *= 1.2;
            if (activeModelParams_.zoomLevel > CpuModel::MAX_ZOOM) activeModelParams_.zoomLevel = CpuModel::MAX_ZOOM;
		}
		else if (event.wheel.y < 0) {
			activeModelParams_.zoomLevel *= 0.8;
			if (activeModelParams_.zoomLevel < CpuModel::MIN_ZOOM) activeModelParams_.zoomLevel = CpuModel::MIN_ZOOM;
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