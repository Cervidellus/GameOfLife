#ifndef GAMEOFLIFE_MODEL_PARAMS
#define GAMEOFLIFE_MODEL_PARAMS

#include <string>
#include <vector>
//For most values, negative values will be ignored
struct ModelParameters {
	bool isRunning = false;
	bool random = true;
	int modelFPS = -1;
	int modelWidth = -1;
	int modelHeight = -1;
	float fillFactor = 0.2f;
	int rule1 = 2; 
	int rule3 = 3; 
	int rule4 = 3;
	int minWidth = 10;
	int minHeight = 10;
	std::vector<std::pair <int, int>> aliveCells = {};
	std::string runLengthEncoding = "";
};

#endif