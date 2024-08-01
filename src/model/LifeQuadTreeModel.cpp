#include "LifeQuadTreeModel.hpp"

#include <random>
#include <iostream>

void LifeQuadTreeModel::initialize()
{
	generateModel_(activeModelParams_);
}	

void LifeQuadTreeModel::update()
{
	//need to update the model
    std::cout << "LifeQuadTreeModel::update() called\n";
}

void LifeQuadTreeModel::handleSDLEvent(const SDL_Event& event)
{
	//need to handle SDL events
    std::cout << "LifeQuadTreeModel::handleSDLEvent() called\n";
}

void LifeQuadTreeModel::draw(
	SDL_Renderer* renderer,
	const int posX,
	const int posY,
	const int width,
	const int height)
{
	//need to draw the model
    std::cout << "LifeQuadTreeModel::draw() called\n";
    //I need to define a rect that gets drawn.
    //const CpuModel::GridDrawRange drawRange = getDrawRange_(width, height);

    //for (int rowIndex = drawRange.rowBegin; rowIndex <= drawRange.rowEnd; rowIndex++)
    //{
    //    for (int columnIndex = drawRange.columnBegin; columnIndex <= drawRange.columnEnd; columnIndex++)
    //    {

    //        SDL_Color color = colorMapper_.ColormapMap[static_cast<ColorMapper::ColormapType>(selectedColorMapIndex_)][grid_[rowIndex][columnIndex]];

    //        SDL_SetRenderDrawColor(
    //            renderer,
    //            color.r,
    //            color.g,
    //            color.b,
    //            255);

    //        SDL_Rect rect =
    //        {
    //            activeModelParams_.zoomLevel * columnIndex + activeModelParams_.displacementX,
    //            activeModelParams_.zoomLevel * rowIndex + activeModelParams_.displacementY,
    //            activeModelParams_.zoomLevel,
    //            activeModelParams_.zoomLevel
    //        };

    //        SDL_RenderFillRect(renderer, &rect);
    //    }
    //}
}

void LifeQuadTreeModel::drawImGuiWidgets(const bool& isModelRunning)
{
	//need to draw the ImGui widgets
    //std::cout << "LifeQuadTreeModel::drawImGuiWidgets() called\n";
}

void LifeQuadTreeModel::setParameters(const ModelParameters& modelParameters)
{
	//need to set the model parameters
    std::cout << "LifeQuadTreeModel::setParameters() called\n";
}

ModelParameters LifeQuadTreeModel::getParameters()
{
	//need to get the model parameters
	return modelParameters;
}

void LifeQuadTreeModel::generateModel_(const ModelParameters& modelParameters)
{
	tree_.clear();

    if (activeModelParams_.modelWidth < modelParameters.minWidth) activeModelParams_.minWidth = modelParameters.minWidth;
    if (activeModelParams_.modelHeight < modelParameters.minHeight) activeModelParams_.minHeight = modelParameters.minHeight;
    if (modelParameters.modelWidth > 0) activeModelParams_.modelWidth = modelParameters.modelWidth;
    if (modelParameters.modelHeight > 0) activeModelParams_.modelHeight = modelParameters.modelHeight;
    if (modelParameters.fillFactor > 0) activeModelParams_.fillFactor = modelParameters.fillFactor;
    if (modelParameters.rule1 > 0) activeModelParams_.rule1 = modelParameters.rule1;
    if (modelParameters.rule3 > 0) activeModelParams_.rule3 = modelParameters.rule3;
    if (modelParameters.rule4 > 0) activeModelParams_.rule4 = modelParameters.rule4;

    if (modelParameters.random) {
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        //Create a grid of modelWidth, modelHeight
        //Fill random values based on fillFactor
        for (int i = 0; i < activeModelParams_.modelWidth; i++) {
            for (int j = 0; j < activeModelParams_.modelHeight; j++) {
                if (distribution(rng) < activeModelParams_.fillFactor) tree_.setLeaf(LifeQuadTree::Point( i, j ), true);
            }
        }
        return;
    }

    //else {
    //    if (!params.runLengthEncoding.empty()) {
    //        std::istringstream rleStream(params.runLengthEncoding);
    //        populateFromRLE_(rleStream);
    //    }
    //}
}