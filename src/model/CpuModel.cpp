#include "CpuModel.hpp"
#include "presets/modelpresets.hpp"
#include "gui/WidgetFunctions.hpp"
#include "../submodules/ImGuiScope/ImguiScope.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"

#include <imgui.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

CpuModel::CpuModel() :
    gridBackBuffer_(nullptr, SDL_DestroyTexture)
{}

void CpuModel::initialize(const SDL_Rect& viewport)
{
    setViewPort(viewport);
    generateModel(activeModelParams_);
}

void CpuModel::setViewPort(const SDL_Rect& viewPort)
{
	viewPort_ = viewPort;
	drawRangeRecalcNeeded_ = true;
}

void CpuModel::resizeGrid_()
{
	currentGrid_.resize(activeModelParams_.modelHeight, std::vector<uint8_t>(activeModelParams_.modelWidth, 0));
    for (auto& row : currentGrid_) {
		row.resize(activeModelParams_.modelWidth, 0);
	}
    drawRangeRecalcNeeded_ = true;
}

void CpuModel::clearGrid_()
{
    for (auto& row : currentGrid_) {
        row.assign(row.size(), 0);
	}
}

void CpuModel::initBackbuffer_(SDL_Renderer* renderer)
{
    
    gridBackBuffer_.reset(
        SDL_CreateTexture(
            renderer,
            pixelFormat_,
            SDL_TEXTUREACCESS_STREAMING,
            activeModelParams_.modelWidth,
            activeModelParams_.modelHeight
        )
    );
    SDL_SetTextureScaleMode(gridBackBuffer_.get(), SDL_SCALEMODE_NEAREST);
    initBackbufferRequired_ = false;
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
    previousGrid_ = currentGrid_;
    int livingNeighbors = 0;
    int rowCount = currentGrid_.size();
    int columnCount = currentGrid_[0].size();
    uint8_t* cellValue;
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            cellValue = &currentGrid_[rowIndex][columnIndex];
            livingNeighbors = 0;

            //count living neighbors with a naive approach that is PLENTY fast enough!
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                int neighborRowIndex = rowIndex + neighborRow;

                //wrap the rows
                if (neighborRowIndex < 0) neighborRowIndex = rowCount - 1;
                if (neighborRowIndex >= rowCount) neighborRowIndex = 0;


                //having the color info combined with the alive state has us need to do some extra if statements. 
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //skip center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;

                    int neighborColumnIndex = columnIndex + neighborColumn;

                    //wrap the columnsa
                    if (neighborColumnIndex < 0) neighborColumnIndex = columnCount - 1;
                    if (neighborColumnIndex >= columnCount) neighborColumnIndex = 0;

                    //count
                    if (previousGrid_[neighborRowIndex][neighborColumnIndex] == aliveValue_) livingNeighbors++;
                }
            }



            if (*cellValue != aliveValue_ && livingNeighbors == activeModelParams_.rule4 ||
                *cellValue == aliveValue_ && std::clamp(livingNeighbors, activeModelParams_.rule1, activeModelParams_.rule3) == livingNeighbors)
            {
                *cellValue = aliveValue_;
            }
            else
            {
                //Cell is dead, so I decrement the value for a nice visualization.
                //a 'normal' GOL would just be setting this to zero, and would use 1 for the alive value.
                *cellValue = (*cellValue >= deadValueDecrement_) ? *cellValue -= deadValueDecrement_ : 0;
            }
        }
    }
}

void CpuModel::draw(SDL_Renderer* renderer)
{
    if (drawRangeRecalcNeeded_) recalcDrawRange_();
    if (initBackbufferRequired_) initBackbuffer_(renderer);

    SDL_SetRenderTarget(renderer, gridBackBuffer_.get());
    Uint16* pixels;
    int pitch = 0;
    SDL_LockTexture(gridBackBuffer_.get(), nullptr, (void**)&pixels, &pitch);

    for (int rowIndex = drawRange_.y; rowIndex <= drawRange_.h; rowIndex++)
    {
        for (int columnIndex = drawRange_.x; columnIndex <= drawRange_.w; columnIndex++)
        {
            SDL_Color color = colorMapper_.getSDLColor(currentGrid_[rowIndex][columnIndex]);

            pixels[rowIndex *currentGrid_[0].size() + columnIndex] = SDL_MapRGB(
                SDL_GetPixelFormatDetails(pixelFormat_),
                NULL,
                color.r,
                color.g,
                color.b);

            if (rowIndex == currentGrid_.size() - 1)
            {
                uint8_t gridValue = currentGrid_[rowIndex][columnIndex];
                SDL_Color pixelValue = colorMapper_.getSDLColor(currentGrid_[rowIndex][columnIndex]);
                uint8_t newR = 0;
                uint8_t newG = 0;
                uint8_t newB = 0;

                int pixelIndex = (rowIndex)*currentGrid_[0].size() + columnIndex;

                SDL_GetRGB(pixels[(rowIndex)*currentGrid_[0].size() + columnIndex],
                    SDL_GetPixelFormatDetails(pixelFormat_),
                    NULL,
                    &newR,
                    &newG,
                    &newB);

                auto newColor = pixels[(rowIndex)*currentGrid_[0].size() + columnIndex];
            }
        }
    }

    SDL_UnlockTexture(gridBackBuffer_.get());

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, gridBackBuffer_.get(), nullptr, &destinationRect_);
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
        drawRangeRecalcNeeded_,
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
//TODO:: Rather than if else statements I might have a map? Or a switch statement
    //I should profile it to see if it would make a difference before changing though. 
    
    //TODO:
    //We only want to get the mouse state if the right events have been called.

    if (!ImGui::IsWindowHovered(4) && !ImGui::IsAnyItemActive())
    {
        float mousePosX, mousePosY;
        int mouseButtonState = SDL_GetMouseState(&mousePosX, &mousePosY);

        if (mouseButtonState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && event.type == SDL_EVENT_MOUSE_MOTION)
        {
      
            activeModelParams_.displacementX += event.motion.xrel;
            activeModelParams_.displacementY += event.motion.yrel;
            //TODO:Check that it is within bounds of a maximum displacement
            drawRangeRecalcNeeded_ = true;
        }
        else if (event.type == SDL_EventType::SDL_EVENT_MOUSE_WHEEL)
        {
            int cursorModelIndexX = (mousePosX - screenSpaceDisplacementX_) / activeModelParams_.zoomLevel;
            int cursorModelIndexY = (mousePosY - screenSpaceDisplacementY_) / activeModelParams_.zoomLevel;

            //Zoom
            if (event.wheel.y > 0) activeModelParams_.zoomLevel += 1;
            else if (event.wheel.y < 0)  activeModelParams_.zoomLevel -= 1;
            activeModelParams_.zoomLevel = std::clamp<double>(activeModelParams_.zoomLevel, MIN_ZOOM, MAX_ZOOM);

            activeModelParams_.displacementX = -cursorModelIndexX * activeModelParams_.zoomLevel + mousePosX  + (activeModelParams_.modelWidth * activeModelParams_.zoomLevel - viewPort_.w )/ 2;
            activeModelParams_.displacementY = -cursorModelIndexY * activeModelParams_.zoomLevel + mousePosY  + (activeModelParams_.modelHeight * activeModelParams_.zoomLevel / 2) - (viewPort_.h / 2);

            drawRangeRecalcNeeded_ = true;
        }
    }
}

void CpuModel::generateModel(const ModelParameters& params) {
    //First set the members to correspond with the parameters
    activeModelParams_.modelWidth = std::max<int>(activeModelParams_.modelWidth, params.minWidth);
    activeModelParams_.modelHeight = std::max<int>(activeModelParams_.modelHeight, params.minHeight);
    activeModelParams_.minWidth = params.minWidth;
    activeModelParams_.minHeight = params.minHeight;

    if (params.modelWidth > 0) activeModelParams_.modelWidth = params.modelWidth;
    if (params.modelHeight > 0) activeModelParams_.modelHeight = params.modelHeight;


    if (params.fillFactor > 0) activeModelParams_.fillFactor = params.fillFactor;
    if (params.rule1 > 0) activeModelParams_.rule1 = params.rule1;
    if (params.rule3 > 0) activeModelParams_.rule3 = params.rule3;
    if (params.rule4 > 0) activeModelParams_.rule4 = params.rule4;

    if (currentGrid_.size() != activeModelParams_.modelHeight || currentGrid_[0].size() != activeModelParams_.modelWidth) {
		resizeGrid_();
	}
    else {
		clearGrid_();
	}

    if (params.random) {
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (auto& row : currentGrid_) {
            for (auto& cell : row) {
				cell = distribution(rng) < params.fillFactor ? aliveValue_ : deadValue_;
			}
		}
        std::cout << "Random model generated" << std::endl;
        drawRangeRecalcNeeded_ = true;
        initBackbufferRequired_ = true;
        return;
    }

    else {
        if (!params.runLengthEncoding.empty()) {
            std::istringstream rleStream(params.runLengthEncoding);
            populateFromRLE_(rleStream);
        }
    }

    initBackbufferRequired_ = true;
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
    if (currentGrid_.size() != activeModelParams_.modelHeight || currentGrid_[0].size() != activeModelParams_.modelWidth) {
        resizeGrid_();
    }
    else {
        clearGrid_();
    }

    int startColumn = (activeModelParams_.modelWidth / 2) - (activeModelParams_.minWidth/2);
    int startRow = (activeModelParams_.modelHeight - activeModelParams_.minHeight) / 2;
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
                currentGrid_[row][column] = aliveValue_;
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

    drawRangeRecalcNeeded_ = true;
    initBackbufferRequired_ = true;
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

void CpuModel::recalcDrawRange_()
{
    //Recalculate screen drawing position and subset of model to draw
    screenSpaceDisplacementX_ = (viewPort_.w / 2) - (activeModelParams_.modelWidth * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementX;
    screenSpaceDisplacementY_ = (viewPort_.h / 2) - (activeModelParams_.modelHeight * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementY;
    drawRange_.x = (screenSpaceDisplacementX_ < 0) ? -screenSpaceDisplacementX_ / activeModelParams_.zoomLevel : 0;//I think this is right but need to incorporate zoom
    drawRange_.y = (screenSpaceDisplacementY_ < 0) ? -screenSpaceDisplacementY_ / activeModelParams_.zoomLevel : 0;
    drawRange_.w = (viewPort_.w - screenSpaceDisplacementX_ <= activeModelParams_.modelWidth) ? viewPort_.w - screenSpaceDisplacementX_ - 1 : activeModelParams_.modelWidth - 1;
    drawRange_.h = (viewPort_.h - screenSpaceDisplacementY_ <= activeModelParams_.modelHeight) ? viewPort_.h - screenSpaceDisplacementY_ - 1 : activeModelParams_.modelHeight - 1;
    drawRange_.w = std::clamp(drawRange_.w, 0, activeModelParams_.modelWidth - 1);
    drawRange_.y = std::clamp(drawRange_.y, 0, activeModelParams_.modelHeight - 1);

    destinationRect_ = SDL_FRect{
    (float)screenSpaceDisplacementX_,
    (float)screenSpaceDisplacementY_,
    (float)currentGrid_[0].size() * activeModelParams_.zoomLevel,
    (float)currentGrid_.size() * activeModelParams_.zoomLevel };

    drawRangeRecalcNeeded_ = false;
}