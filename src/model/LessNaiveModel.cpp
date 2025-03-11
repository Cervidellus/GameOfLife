#include "model/LessNaiveModel.hpp"
#include "presets/modelpresets.hpp"
#include "gui/WidgetFunctions.hpp"
#include "../submodules/ImGuiScope/ImguiScope.hpp"
#include "../submodules/portable-file-dialogs/portable-file-dialogs.h"

#include <imgui.h>

#include <algorithm>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

LessNaiveModel::LessNaiveModel() 
{
}

void LessNaiveModel::setCell(int x, int y, uint8_t value)
{
    currentGrid_(x, y) = value;
}

uint8_t LessNaiveModel::getCell(int x, int y)
{
    return currentGrid_(x, y);
}

size_t LessNaiveModel::dataColumns() { return currentGrid_.columns(); }

size_t LessNaiveModel::dataRows() { return currentGrid_.rows(); }

void LessNaiveModel::resizeGrid_()
{
    currentGrid_.resize(activeModelParams_.modelWidth, activeModelParams_.modelHeight);
    previousGrid_.resize(activeModelParams_.modelWidth, activeModelParams_.modelHeight);
    drawRangeRecalcNeeded_ = true;
}

void LessNaiveModel::clearGrid_()
{
    currentGrid_.zero();
    previousGrid_.zero();
}

void LessNaiveModel::updateCell_(int col, int row, int previousValue, int neighborCount)
{
    if ((previousValue != aliveValue_ && neighborCount == activeModelParams_.rule4) ||
        (previousValue == aliveValue_ && std::clamp(neighborCount, activeModelParams_.rule1, activeModelParams_.rule3) == neighborCount))
    {
        currentGrid_(col, row) = aliveValue_;
    }
    else
    {
        //Cell is dead, so I decrement the value for a nice visualization where values less than 255 recieve a different color.
        //a 'normal' GOL would just be setting this to zero, and would use 1 for the alive value.
        currentGrid_(col, row) = (previousValue >= deadValueDecrement_) ? previousValue - deadValueDecrement_ : 0;
    }
}

void LessNaiveModel::update()
{
    previousGrid_.swap(currentGrid_);
    int livingNeighbors = 0;
    int rowCount = currentGrid_.rows();
    int columnCount = currentGrid_.columns();
    uint8_t cellValue;

    //Process center of matrix, ignoring edges
    for (size_t rowIndex = 1; rowIndex < rowCount -1; rowIndex++) {
        for (size_t columnIndex = 1; columnIndex < columnCount -1; columnIndex++) {
            cellValue = previousGrid_(columnIndex , rowIndex);
            livingNeighbors = 0;

            livingNeighbors += previousGrid_(columnIndex - 1, rowIndex - 1) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex, rowIndex - 1) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex + 1, rowIndex - 1) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex - 1, rowIndex) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex + 1, rowIndex) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex -1, rowIndex + 1) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex, rowIndex + 1) == aliveValue_ ? 1 : 0;
            livingNeighbors += previousGrid_(columnIndex + 1, rowIndex + 1) == aliveValue_ ? 1 : 0;

            updateCell_(columnIndex, rowIndex, cellValue, livingNeighbors);
        }
    }

    int bottomRowIndex = rowCount - 1;
    int rightColumnIndex = columnCount - 1;
    //Top row
    for (size_t columnIndex = 1; columnIndex < columnCount - 1; columnIndex++) {
        cellValue = previousGrid_(columnIndex, 0);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(columnIndex - 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex - 1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex - 1, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, 1) == aliveValue_ ? 1 : 0;

        updateCell_(columnIndex, 0, cellValue, livingNeighbors);
    }

    //Bottom Row
    for (size_t columnIndex = 1; columnIndex < columnCount - 1; columnIndex++) {
        cellValue = previousGrid_(columnIndex, bottomRowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(columnIndex - 1, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex - 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex - 1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(columnIndex + 1, 0) == aliveValue_ ? 1 : 0;

        updateCell_(columnIndex, bottomRowIndex, cellValue, livingNeighbors);
    }

    //Left column
    for (size_t rowIndex = 1; rowIndex < rowCount - 1; rowIndex++)
    {
        cellValue = previousGrid_(0, rowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, rowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, rowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, rowIndex + 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, rowIndex + 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, rowIndex + 1) == aliveValue_ ? 1 : 0;

        updateCell_(0, rowIndex, cellValue, livingNeighbors);
    }

    //Right column
    for (size_t rowIndex = 1; rowIndex < rowCount - 1; rowIndex++)
    {
        cellValue = previousGrid_(0, rowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex -1, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, rowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, rowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, rowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, rowIndex + 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, rowIndex + 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, rowIndex + 1) == aliveValue_ ? 1 : 0;

        updateCell_(rightColumnIndex, rowIndex, cellValue, livingNeighbors);
    }

    //Top Left
    {
        cellValue = previousGrid_(0, 0);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, 1) == aliveValue_ ? 1 : 0;

        updateCell_(0, 0, cellValue, livingNeighbors);
    }

    //Bottom Left
    {
        cellValue = previousGrid_(0, bottomRowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(1, 0) == aliveValue_ ? 1 : 0;

        updateCell_(0, bottomRowIndex, cellValue, livingNeighbors);
    }

    //Top Right
    {
        cellValue = previousGrid_(0, bottomRowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex - 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, 1) == aliveValue_ ? 1 : 0;

        updateCell_(rightColumnIndex, 0, cellValue, livingNeighbors);
    }

    //Bottom Right
    {
        cellValue = previousGrid_(0, bottomRowIndex);
        livingNeighbors = 0;

        livingNeighbors += previousGrid_(rightColumnIndex - 1, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, bottomRowIndex - 1) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, bottomRowIndex) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex - 1, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(rightColumnIndex, 0) == aliveValue_ ? 1 : 0;
        livingNeighbors += previousGrid_(0, 0) == aliveValue_ ? 1 : 0;

        updateCell_(rightColumnIndex, bottomRowIndex, cellValue, livingNeighbors);
    }
}

void LessNaiveModel::draw(SDL_Renderer* renderer)
{
    if (drawRangeRecalcNeeded_) recalcDrawRange_();
    if (initBackbufferRequired_) initBackbuffer_(renderer);

    SDL_SetRenderTarget(renderer, gridBackBuffer_.get());
    Uint16* pixels;
    int pitch = 0;
    SDL_LockTexture(gridBackBuffer_.get(), nullptr, (void**)&pixels, &pitch);
    SDL_Color color;
    for (int rowIndex = drawRange_.y; rowIndex <= drawRange_.h; rowIndex++)
    {
        for (int columnIndex = drawRange_.x; columnIndex <= drawRange_.w; columnIndex++)
        {
            color = colorMapper_.getSDLColor(currentGrid_(columnIndex, rowIndex));

            pixels[(rowIndex)*currentGrid_.columns() + columnIndex] = SDL_MapRGB(
                formatDetails_,
                nullptr,
                color.r,
                color.g,
                color.b);
        }
    }

    SDL_UnlockTexture(gridBackBuffer_.get());
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, gridBackBuffer_.get(), nullptr, &destinationRect_);
}

void LessNaiveModel::drawImGuiWidgets(const bool& isModelRunning)
{
    WidgetFunctions::drawGOLRulesHeader(
        activeModelParams_,
        [this](const ModelParameters& params) {generateModel(params); },
        isModelRunning);

    WidgetFunctions::drawVisualizationHeader(
        activeModelParams_,
        colorMapper_,
        deadValueDecrement_,
        drawRangeRecalcNeeded_,
        isModelRunning);

    WidgetFunctions::drawPresetsHeader(
        activeModelParams_,
        [this](const ModelParameters& params) {generateModel(params); },
        [this](std::string filePath) {loadRLE_(filePath); },
        [this]() {populateFromRLEString_(inputString_); },
        inputString_,
        isModelRunning
    );
}

void LessNaiveModel::handleSDLEvent(const SDL_Event& event)
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

            activeModelParams_.displacementX = -cursorModelIndexX * activeModelParams_.zoomLevel + mousePosX + (activeModelParams_.modelWidth * activeModelParams_.zoomLevel - viewPort_.w) / 2;
            activeModelParams_.displacementY = -cursorModelIndexY * activeModelParams_.zoomLevel + mousePosY + (activeModelParams_.modelHeight * activeModelParams_.zoomLevel / 2) - (viewPort_.h / 2);

            drawRangeRecalcNeeded_ = true;
        }
    }
}
