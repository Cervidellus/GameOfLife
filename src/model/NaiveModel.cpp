#include "NaiveModel.hpp"
#include "presets/modelpresets.hpp"
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