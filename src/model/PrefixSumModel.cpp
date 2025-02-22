#include "model/PrefixSumModel.hpp"
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

PrefixSumModel::PrefixSumModel() :
    gridBackBuffer_(nullptr, SDL_DestroyTexture)
{
}

void PrefixSumModel::initialize(const SDL_Rect& viewport)
{
    setViewPort(viewport);
    generateModel(activeModelParams_);
    recalcDrawRange_ = true;
}

void PrefixSumModel::setViewPort(const SDL_Rect& viewPort)
{
    viewPort_ = viewPort;
    recalcDrawRange_ = true;
}

void PrefixSumModel::resizeGrid_()
{
    currentGrid_.resize(activeModelParams_.modelWidth, activeModelParams_.modelHeight);
    previousGrid_.resize(activeModelParams_.modelWidth, activeModelParams_.modelHeight);
    colorGrid_.resize(activeModelParams_.modelWidth, activeModelParams_.modelHeight);
    //I should  switch to std::mdspan rather than the drawRange_.
    recalcDrawRange_ = true;
}

void PrefixSumModel::clearGrid_()
{
    currentGrid_.zero();
    previousGrid_.zero();
    colorGrid_.zero();
}

void PrefixSumModel::initBackbuffer_(SDL_Renderer* renderer)
{

    gridBackBuffer_.reset(
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            activeModelParams_.modelWidth,
            activeModelParams_.modelHeight
        )
    );
    SDL_SetTextureScaleMode(gridBackBuffer_.get(), SDL_SCALEMODE_NEAREST);
    initBackbufferRequired_ = false;
}

void PrefixSumModel::setParameters(const ModelParameters& modelParameters)
{
    activeModelParams_ = modelParameters;
}

ModelParameters PrefixSumModel::getParameters()
{
    return activeModelParams_;
}

void PrefixSumModel::update()
{
    previousGrid_ = currentGrid_;
    int livingNeighbors = 0;
    int rowCount = currentGrid_.rows();
    int columnCount = currentGrid_.columns();
    for (size_t rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        for (size_t columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            uint8_t& cellValue = currentGrid_(columnIndex, rowIndex);
            livingNeighbors = 0;

            //count living neighbors with a naive approach that is PLENTY fast enough!
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                int neighborRowIndex = rowIndex + neighborRow;

                //wrap the rows
                if (neighborRowIndex < 0) neighborRowIndex = rowCount - 1;
                if (neighborRowIndex >= rowCount) neighborRowIndex = 0;


                //having the color info combined with the alive state has us need to do some unneccessary if statements. 
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //skip center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;

                    int neighborColumnIndex = columnIndex + neighborColumn;

                    //wrap the columnsa
                    if (neighborColumnIndex < 0) neighborColumnIndex = columnCount - 1;
                    if (neighborColumnIndex >= columnCount) neighborColumnIndex = 0;

                    //count
                    if (previousGrid_(columnIndex, rowIndex) == aliveValue_) livingNeighbors++;
                }
            }



            if (cellValue != aliveValue_ && livingNeighbors == activeModelParams_.rule4 ||
                cellValue == aliveValue_ && std::clamp(livingNeighbors, activeModelParams_.rule1, activeModelParams_.rule3) == livingNeighbors)
            {
                cellValue = aliveValue_;
                //*cellValue = aliveValue_;
            }
            else
            {
                //Cell is dead, so I decrement the value for a nice visualization.
                //a 'normal' GOL would just be setting this to zero, and would use 1 for the alive value.
                cellValue = (cellValue >= deadValueDecrement_) ? cellValue -= deadValueDecrement_ : 0;
            }
        }
    }
}

void PrefixSumModel::draw(SDL_Renderer* renderer)
{
    //This should just be what is written to activeModelParams_.displacementX and activeModelParams_.displacementY
    //For that I'll need to grab window resize events.

    if (recalcDrawRange_) {
        screenSpaceDisplacementX_ = (viewPort_.w / 2) - (activeModelParams_.modelWidth * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementX;
        screenSpaceDisplacementY_ = (viewPort_.h / 2) - (activeModelParams_.modelHeight * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementY;
        drawRange_ = getDrawRange_();
        recalcDrawRange_ = false;
    }

    if (initBackbufferRequired_) initBackbuffer_(renderer);

    //****Drawing by swapping my backbuffer****
    // NEXT:
    //-I should have it check for changes in model, so I can skip rendering step when rendering is higher frequency than model.

    if (!gridBackBuffer_) {
        std::cout << "Invalid backbuffer!\n";
        return;
    }
    auto drawBackBufferTimer = std::make_optional<ImGuiScope::TimeScope>("Draw My Backbuffer");

    SDL_SetRenderTarget(renderer, gridBackBuffer_.get());
    Uint16* pixels;
    int pitch = 0;
    SDL_LockTexture(gridBackBuffer_.get(), nullptr, (void**)&pixels, &pitch);

    for (int rowIndex = 0; rowIndex < currentGrid_.rows(); rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < currentGrid_.columns(); columnIndex++)
        {
            SDL_Color color = colorMapper_.getSDLColor(currentGrid_(columnIndex, rowIndex));

            pixels[(rowIndex)*currentGrid_.columns() + columnIndex] = SDL_MapRGB(
                SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGB565),
                nullptr,
                color.r,
                color.g,
                color.b);
        }
    }

    SDL_UnlockTexture(gridBackBuffer_.get());
    SDL_SetRenderTarget(renderer, nullptr);
    auto destRect = SDL_FRect{
        (float)screenSpaceDisplacementX_,
        (float)screenSpaceDisplacementY_,
        (float)currentGrid_.columns() * activeModelParams_.zoomLevel,
        (float)currentGrid_.rows() * activeModelParams_.zoomLevel };
    SDL_RenderTexture(renderer, gridBackBuffer_.get(), nullptr, &destRect);
    drawBackBufferTimer.reset();
}

//typedef struct SDL_FRect
//{
//    float x;
//    float y;
//    float w;
//    float h;
//} SDL_FRect;



void PrefixSumModel::drawImGuiWidgets(const bool& isModelRunning)
{
    WidgetFunctions::drawGOLRulesHeader(
        activeModelParams_,
        [this](const ModelParameters& params) {generateModel(params); },
        isModelRunning);

    WidgetFunctions::drawVisualizationHeader(
        activeModelParams_,
        colorMapper_,
        deadValueDecrement_,
        recalcDrawRange_,
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

void PrefixSumModel::handleSDLEvent(const SDL_Event& event)
{
    //TODO:: Rather than if else statements I might have a map? Or a switch statement
        //I should profile it to see if it would make a difference before changing though. 

        //TODO:
        //We only want to get the mouse state if the right events have been called.

    if (!ImGui::IsWindowHovered(4) && !ImGui::IsAnyItemActive())
    {
        float mousePosX, mousePosY;
        int mouseButtonState = SDL_GetMouseState(&mousePosX, &mousePosY);

        if (mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) && event.type == SDL_EVENT_MOUSE_MOTION)
        {

            activeModelParams_.displacementX += event.motion.xrel;
            activeModelParams_.displacementY += event.motion.yrel;
            //TODO:Check that it is within bounds of a maximum displacement
            recalcDrawRange_ = true;
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

            recalcDrawRange_ = true;
        }
    }
}

void PrefixSumModel::generateModel(const ModelParameters& params) {
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

    if (currentGrid_.rows() != activeModelParams_.modelHeight || currentGrid_.columns() != activeModelParams_.modelWidth) {
        resizeGrid_();
    }
    else {
        clearGrid_();
    }

    if (params.random) {
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (auto& cell : currentGrid_.vector)
        {
            cell = distribution(rng) < params.fillFactor ? aliveValue_ : deadValue_;
        }

        std::cout << "Random model generated" << std::endl;
        recalcDrawRange_ = true;
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

void PrefixSumModel::populateFromRLE_(std::istream& modelStream)
{
    std::string line = "";
    std::string RLEstring = "";
    while (std::getline(modelStream, line))
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
    activeModelParams_.modelHeight = std::max<int>(activeModelParams_.modelHeight, activeModelParams_.minHeight);
    if (currentGrid_.rows() != activeModelParams_.modelHeight || currentGrid_.columns()!= activeModelParams_.modelWidth) {
        resizeGrid_();
    }
    else {
        clearGrid_();
    }

    int startColumn = (activeModelParams_.modelWidth / 2) - (activeModelParams_.minWidth / 2);
    int startRow = (activeModelParams_.modelHeight - activeModelParams_.minHeight) / 2;
    int row = startRow;
    int column = startColumn;

    std::string::iterator RLEit;
    for (std::string::iterator it = RLEstring.begin(); it != RLEstring.end(); ++it)
    {
        if (*it == '!') break;

        std::string stringInteger = "";
        while (isdigit(*it) || *it == '\n')
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
                currentGrid_(column, row) = aliveValue_;
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

    recalcDrawRange_ = true;
    initBackbufferRequired_ = true;
}

void PrefixSumModel::loadRLE_(const std::string& filePath)
{
    std::ifstream filestream(filePath);
    if (filestream.is_open())
    {
        clearGrid_();
        populateFromRLE_(filestream);
        filestream.close();
    }
}

void PrefixSumModel::populateFromRLEString_(const std::string& rleString)
{
    std::stringstream rleStream(rleString);
    populateFromRLE_(rleStream);
}

//I should only be calculating this when it changes.
PrefixSumModel::GridDrawRange PrefixSumModel::getDrawRange_()
{
    PrefixSumModel::GridDrawRange drawRange;
    drawRange.rowBegin = -(screenSpaceDisplacementY_ + activeModelParams_.displacementY) / activeModelParams_.zoomLevel;
    drawRange.rowEnd = drawRange.rowBegin + (viewPort_.h / activeModelParams_.zoomLevel);
    drawRange.columnBegin = -(screenSpaceDisplacementX_ + activeModelParams_.displacementX) / activeModelParams_.zoomLevel;
    drawRange.columnEnd = drawRange.columnBegin + (viewPort_.w / activeModelParams_.zoomLevel);

    //Don't try and draw something not in grid_
    int gridRows = currentGrid_.rows();
    int gridColumns = currentGrid_.columns();
    if (drawRange.rowEnd >= gridRows) drawRange.rowEnd = gridRows - 1;
    if (drawRange.columnEnd >= gridColumns) drawRange.columnEnd = gridColumns - 1;
    if (drawRange.rowBegin < 0) drawRange.rowBegin = 0;
    if (drawRange.columnBegin < 0) drawRange.columnBegin = 0;

    return drawRange;
}