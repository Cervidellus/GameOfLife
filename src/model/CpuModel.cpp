#include "CpuModel.h"

#include <random>

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

void CpuModel::update()
{
    std::vector<std::vector<uint8_t>> previousState = grid_;
    int livingNeighbors = 0;
    bool cellAlive = false;
    for (int rowIndex = 0; rowIndex < grid_.size(); rowIndex++) {
        for (int columnIndex = 0; columnIndex < grid_[rowIndex].size(); columnIndex++) {
            cellAlive = (grid_[rowIndex][columnIndex] == aliveValue_) ? aliveValue_ : deadValue_;
            livingNeighbors = 0;
            //count living neighbors
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                int neighborRowIndex = rowIndex + neighborRow;
                //wrap the rows
                if (neighborRowIndex < 0) neighborRowIndex = grid_.size() - 1;
                if (neighborRowIndex >= grid_.size()) neighborRowIndex = 0;

                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //skip center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;
                    neighborColumn = columnIndex + neighborColumn;

                    //wrap the columns
                    if (neighborColumn < 0) neighborColumn = grid_[rowIndex].size() - 1;
                    if (neighborColumn >= grid_[rowIndex].size()) neighborColumn = 0;

                    //count
                    if (previousState[neighborRowIndex][neighborColumn] == aliveValue_) livingNeighbors++;
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

//
//
//
//
//void Core::handleGenerateModelRequest(const ModelParameters& params) {
//    surface_ = generateModelPresetSurface(params);
//}


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

        for (auto row : grid_) {
            for (auto cell : row) {
				cell = distribution(rng) < params.fillFactor ? 255 : 0;
			}
		}
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