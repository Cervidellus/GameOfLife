#include "CpuModel.hpp"

#include <iostream>
#include <random>

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
    
    //First I can iterate over all and see if it works.
    //Then I can put in place what i need to only draw what is on screen.
    //Then I can add the ability to zoom.
    //Then I can add the ability to pan.
    
    //Figure out number of rows and columns to draw
    int rowsToDraw = std::min<int>(grid_.size(), height);
    int columnsToDraw = std::min<int>(grid_[0].size(), width);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(int rowIndex = 0; rowIndex < rowsToDraw; rowIndex++)
	{
		for(int columnIndex = 0; columnIndex < columnsToDraw; columnIndex++)
		{
			if(grid_[rowIndex][columnIndex] == aliveValue_)
			{
				//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = { posX + columnIndex, posY + rowIndex, 1, 1 };
                SDL_RenderFillRect(renderer, &rect);
			}


		}
	}
}

void CpuModel::generateModel_(const ModelParameters& params) {
    //First set the members to correspond with the parameters
    if (params.modelFPS > 0) activeModelParams_.modelFPS = params.modelFPS;
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