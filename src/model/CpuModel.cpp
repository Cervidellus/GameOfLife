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
    if(width != viewportWidth_ || height != viewportHeight_)
	{
		viewportWidth_ = width;
		viewportHeight_ = height;
	}  

    int gridRows = grid_.size();
    int gridColumns = grid_[0].size();
    const float rectPadding = 0.1;//not used yet
    const float scale = 1.0;
    
    //Figure out the starting indices and ending indices to draw, so we only draw what is on screen.
    int rowsToDrawCount = height / activeModelParams_.zoomLevel;
    int columnsToDrawCount = width / activeModelParams_.zoomLevel;
    int columnDrawStartIndex = -(int)(((double)activeModelParams_.displacementX / activeModelParams_.zoomLevel));
    int rowDrawStartIndex = -(int)(((double)activeModelParams_.displacementY / activeModelParams_.zoomLevel));
    
    int rowDrawEndIndex = rowDrawStartIndex + rowsToDrawCount - 1;
    if(rowDrawEndIndex >= gridRows) rowDrawEndIndex = gridRows - 1;
    int columnDrawEndIndex = columnDrawStartIndex + columnsToDrawCount - 1;
    if(columnDrawEndIndex >= gridColumns) columnDrawEndIndex = gridColumns - 1;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(int rowIndex = rowDrawStartIndex; rowIndex <= rowDrawEndIndex; rowIndex++)
	{
        if (rowIndex < 0) continue;
		for(int columnIndex = columnDrawStartIndex; columnIndex <= columnDrawEndIndex; columnIndex++)
		{
            if (columnIndex < 0) continue;
			if(grid_[rowIndex][columnIndex] == aliveValue_)
			{
                //I might be able to add the remainder of the displacement to get smoother panning...
                SDL_Rect rect = 
                { 
                    activeModelParams_.zoomLevel * (columnIndex - columnDrawStartIndex),
                    activeModelParams_.zoomLevel * (rowIndex - rowDrawStartIndex),
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

//TODO:: I might not need the renderer.
void CpuModel::handleSDLEvent(
    const SDL_Event& event
    //const int& mousePosX,
    //const int& mousePosY,
    //const int& mouseButtonState
    //const bool& isCursorInOverlay
)
{
 
    //TODO::I need to control behavior to know if the mouse is inside or outside of the IMGUI menu.

    //I can get the mouse state here. 
    //I can either have CpuModel have a callback so it can grab the info when it needs it.
    //This will reduce the number of calls to get the status, but makes code more complicated and less readable.
    //Or I can just pass a rect every time the method is called. This results in a small allocation every time an event is passed, which is constantly. 
    //I could limit the types of events that get passed to CpuHandler to reduce some calls.. honestly it is probably not a big deal.
    ////
    int mousePosX, mousePosY;
    int mouseButtonState = SDL_GetMouseState(&mousePosX, &mousePosY);
    //////auto widgetViewport = ImGui::GetMainViewport();
    //////ImGui::GetCurrentContext();
    //////auto imGuiMin = ImGui::GetWindowContentRegionMin();//exception because frame is not currently being drawn.
    //////auto imGuiMax = ImGui::GetWindowContentRegionMax();//exception because frame is not currently being drawn.
    ////auto imGuiSize = ImGui::GetIO().DisplaySize;
    ////auto imGuiWindowPos = ImGui::GetWindowPos();//exception because frame is not currently being drawn.

    ////auto drawData = ImGui::GetDrawData();
    ////auto disSize = drawData->DisplaySize;//The whole window
    ////auto disPos = drawData->DisplayPos;//the whole window

    //bool isPointInOverlay = false;
    ////if (x >= imGuiMin.x && x <= imGuiMax.x && y >= imGuiMin.y && y <= imGuiMax.y) isPointInOverlay = true;

    //widgetViewport.
    if (!ImGui::IsAnyItemActive())
    {
        if (mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) && event.type == SDL_MOUSEMOTION)
        {

            //TODO: should I make sure that the mouse is in the viewport space? 
            //Change displacement
            activeModelParams_.displacementX += event.motion.xrel;
            activeModelParams_.displacementY += event.motion.yrel;
            //Check that it is within bounds of a maximum displacement


        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            //Zoom
            //TODO:: only allow scales that are a multiple of pixels.
            double newZoomLevel = activeModelParams_.zoomLevel;
            if (event.wheel.y > 0)
            {
                newZoomLevel = activeModelParams_.zoomLevel * 1.2;
                if (activeModelParams_.zoomLevel > CpuModel::MAX_ZOOM) newZoomLevel = CpuModel::MAX_ZOOM;
            }
            else if (event.wheel.y < 0) {
                newZoomLevel = activeModelParams_.zoomLevel * 0.8;
                if (activeModelParams_.zoomLevel < CpuModel::MIN_ZOOM) newZoomLevel = CpuModel::MIN_ZOOM;
            }

            //map the cursor position in viewport space to the index in model space. Zoomlevel is the width of 1 cell in viewport space.
            //int mouseX, mouseY;
            //SDL_GetMouseState(&mouseX, &mouseY);//WindowSpace
            int cursorModelIndexX = (int)(((double)mousePosX - (double)activeModelParams_.displacementX) / activeModelParams_.zoomLevel);
            int cursorModelIndexY = (int)(((double)mousePosY - (double)activeModelParams_.displacementY) / activeModelParams_.zoomLevel);

            //Calculate the new displacement, trying to keep the model in the same position relative to the cursor. 
            //This works MOSTLY but drifts more than I would like. 
            activeModelParams_.displacementX = (int)(-((double)cursorModelIndexX * newZoomLevel) + (double)mousePosX);
            activeModelParams_.displacementY = (int)(-((double)cursorModelIndexY * newZoomLevel) + (double)mousePosY);

            activeModelParams_.zoomLevel = newZoomLevel;
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