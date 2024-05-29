#ifndef GAMEOFLIFE_MODEL_PARAMS
#define GAMEOFLIFE_MODEL_PARAMS

#include <vector>
//For most values, negative values will be ignored
struct ModelParameters {
	bool random = true;
	int modelFPS = -1;
	int modelWidth = 500;
	int modelHeight = 500;
	float fillFactor = 0.2f;
	int rule1 = 2; 
	int rule3 = 3; 
	int rule4 = 3;
	int minWidth = 10;
	int minHeight = 10;
	std::vector<std::pair <int, int>> aliveCells = {};
};

#endif