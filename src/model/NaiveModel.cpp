#include "NaiveModel.hpp"
#include "presets/modelpresets.hpp"
#include "gui/WidgetFunctions.hpp"
#include "../submodules/ImGuiScope/ImguiScope.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"

#include <imgui.h>

#include <algorithm>
#include <iostream>
#include <random>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

NaiveModel::NaiveModel()
{}

void NaiveModel::setCell(int x, int y, uint8_t value)
{
    currentGrid_[y][x] = value;
}

uint8_t NaiveModel::getCell(int x, int y) 
{
    return currentGrid_[y][x];
}

size_t NaiveModel::dataColumns()
{
    return currentGrid_[0].size();
}

size_t NaiveModel::dataRows()
{
    return currentGrid_.size();
}

void NaiveModel::resizeGrid_()
{
	currentGrid_.resize(activeModelParams_.modelHeight, std::vector<uint8_t>(activeModelParams_.modelWidth, 0));
    for (auto& row : currentGrid_) {
		row.resize(activeModelParams_.modelWidth, 0);
	}
    drawRangeRecalcNeeded_ = true;
}

void NaiveModel::clearGrid_()
{
    for (auto& row : currentGrid_) {
        row.assign(row.size(), 0);
	}
}

void NaiveModel::update()
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

void NaiveModel::draw(SDL_Renderer* renderer)
{
    if (drawRangeRecalcNeeded_) recalcDrawRange_();
    if (initBackbufferRequired_) initBackbuffer_(renderer);

    SDL_SetRenderTarget(renderer, gridBackBuffer_.get());
    Uint16* pixels;
    int pitch = 0;
    SDL_Color color;
    SDL_LockTexture(gridBackBuffer_.get(), nullptr, (void**)&pixels, &pitch);

    for (int rowIndex = drawRange_.y; rowIndex <= drawRange_.h; rowIndex++)
    {
        for (int columnIndex = drawRange_.x; columnIndex <= drawRange_.w; columnIndex++)
        {
            color = colorMapper_.getSDLColor(currentGrid_[rowIndex][columnIndex]);

            pixels[rowIndex *currentGrid_[0].size() + columnIndex] = SDL_MapRGB(
                formatDetails_,
                NULL,
                color.r,
                color.g,
                color.b);
        }
    }

    SDL_UnlockTexture(gridBackBuffer_.get());

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, gridBackBuffer_.get(), nullptr, &destinationRect_);
}

void NaiveModel::drawImGuiWidgets(const bool& isModelRunning)
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

void NaiveModel::handleSDLEvent(const SDL_Event& event)
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
