#include "CpuModel.hpp"
#include "presets/modelpresets.hpp"
#include "gui/WidgetFunctions.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include <imgui.h>
#include <SDL.h>

void CpuModel::initialize()
{
    generateModel(activeModelParams_);
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
            }
            //If neighbors are less than 2 or more than 3, kill it.
            else if (livingNeighbors < activeModelParams_.rule1 || livingNeighbors > activeModelParams_.rule3) {
                *cellValue = (*cellValue >= deadValueDecrement_) ? *cellValue -= deadValueDecrement_ : 0;
            }
        }
    }
}

void CpuModel::draw(SDL_Renderer* renderer, const int posX, const int posY, const int width, const int height)
{
    //TODO: It is redrawing the model every time. It might be better to draw it to a texture and reuse the texture if model is not updated.
    //I might even update the texture at the same time I update the model, if the view has drawn the last one. 
    //THe hard thing there is making sure that we aren't drawing the texture more often than we need. 
    //I could have a bool that says the texture needs updating before drawing and handle that in core.cpp.

    const CpuModel::GridDrawRange drawRange = getDrawRange_(width, height);

    for (int rowIndex = drawRange.rowBegin; rowIndex <= drawRange.rowEnd; rowIndex++)
    {
        for (int columnIndex = drawRange.columnBegin; columnIndex <= drawRange.columnEnd; columnIndex++)
        {
            
            SDL_Color color = colorMapper_.getSDLColor(grid_[rowIndex][columnIndex]);

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
    WidgetFunctions::drawGOLRulesHeader(
        activeModelParams_, 
        [this](const ModelParameters& params) {generateModel(params);},
        isModelRunning);

    WidgetFunctions::drawVisualizationHeader(
		activeModelParams_,
		colorMapper_,
		deadValueDecrement_,
		isModelRunning);
    
    WidgetFunctions::drawPresetsHeader(
		activeModelParams_,
		[this](const ModelParameters& params) {generateModel(params);},
		[this](std::string filePath) {loadRLE_(filePath);},
        [this]() {populateFromRLEString_(inputString_);},
        inputString_,
        isModelRunning
        );
}

void CpuModel::handleSDLEvent(const SDL_Event& event)
{
    int mousePosX, mousePosY;
    int mouseButtonState = SDL_GetMouseState(&mousePosX, &mousePosY);
    
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

void CpuModel::generateModel(const ModelParameters& params) {
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

void CpuModel::loadRLE_(const std::string& filePath)
{
	std::ifstream filestream(filePath);
	if (filestream.is_open())
	{
		clearGrid_();
		populateFromRLE_(filestream);
		filestream.close();
	}
}

void CpuModel::populateFromRLEString_(const std::string& rleString)
{
	std::stringstream rleStream(rleString);
	populateFromRLE_(rleStream);
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