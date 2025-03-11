#include "abstract_model.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

AbstractModel::AbstractModel() : gridBackBuffer_(nullptr, SDL_DestroyTexture) {}

void AbstractModel::initialize(const SDL_Rect& viewport)
{
    setViewPort(viewport);
    generateModel(activeModelParams_);
}

void AbstractModel::setViewPort(const SDL_Rect& viewPort)
{
    viewPort_ = viewPort;
    drawRangeRecalcNeeded_ = true;
}

void AbstractModel::setParameters(const ModelParameters& modelParameters)
{
    activeModelParams_ = modelParameters;
}

ModelParameters AbstractModel::getParameters()
{
    return activeModelParams_;
}

void AbstractModel::generateModel(const ModelParameters& params) {
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

    if (dataRows() != activeModelParams_.modelHeight || dataColumns() != activeModelParams_.modelWidth) {
        resizeGrid_();
    }
    else {
        clearGrid_();
    }

    if (params.random) {
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (int row = 0; row < dataRows(); row++)
        {
            for (int col = 0; col < dataColumns(); col++)
            {
                setCell(col, row, distribution(rng) < params.fillFactor ? aliveValue_ : deadValue_);
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

void AbstractModel::recalcDrawRange_()
{
    screenSpaceDisplacementX_ = (viewPort_.w / 2) - (activeModelParams_.modelWidth * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementX;
    screenSpaceDisplacementY_ = (viewPort_.h / 2) - (activeModelParams_.modelHeight * activeModelParams_.zoomLevel / 2) + activeModelParams_.displacementY;
    drawRange_.x = (screenSpaceDisplacementX_ < 0) ? -screenSpaceDisplacementX_ / activeModelParams_.zoomLevel : 0;
    drawRange_.y = (screenSpaceDisplacementY_ < 0) ? -screenSpaceDisplacementY_ / activeModelParams_.zoomLevel : 0;
    drawRange_.w = (viewPort_.w - screenSpaceDisplacementX_ <= activeModelParams_.modelWidth) ? viewPort_.w - screenSpaceDisplacementX_ - 1 : activeModelParams_.modelWidth - 1;
    drawRange_.h = (viewPort_.h - screenSpaceDisplacementY_ <= activeModelParams_.modelHeight) ? viewPort_.h - screenSpaceDisplacementY_ - 1 : activeModelParams_.modelHeight - 1;
    drawRange_.w = std::clamp(drawRange_.w, 0, activeModelParams_.modelWidth - 1);
    drawRange_.y = std::clamp(drawRange_.y, 0, activeModelParams_.modelHeight - 1);

    destinationRect_ = SDL_FRect{
        (float)screenSpaceDisplacementX_,
        (float)screenSpaceDisplacementY_,
        (float)activeModelParams_.modelWidth * activeModelParams_.zoomLevel,
        (float)activeModelParams_.modelHeight * activeModelParams_.zoomLevel };

    drawRangeRecalcNeeded_ = false;
}

void AbstractModel::populateFromRLE_(std::istream& modelStream)
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
    if (dataRows() != activeModelParams_.modelHeight || dataColumns() != activeModelParams_.modelWidth) {
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
                setCell(column, row, aliveValue_);
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

//Load an RLE file and populate the board. Intended as a callback sent to gui.
void AbstractModel::loadRLE_(const std::string& filePath)
{
    std::ifstream filestream(filePath);
    if (filestream.is_open())
    {
        clearGrid_();
        populateFromRLE_(filestream);
        filestream.close();
    }
}

//Convert and RLE string to a stream and call populateFromRLE_
void AbstractModel::populateFromRLEString_(const std::string& rleString)
{
    std::stringstream rleStream(rleString);
    populateFromRLE_(rleStream);
}

void AbstractModel::initBackbuffer_(SDL_Renderer* renderer)
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