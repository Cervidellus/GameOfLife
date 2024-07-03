#ifndef GAMEOFLIFE_MODEL_PARAMS
#define GAMEOFLIFE_MODEL_PARAMS

#include <string>
#include <vector>
//For most values, negative values will be ignored
struct ModelParameters {
	//bool isRunning = false;//Pull into core
	bool random = true;
	//int modelFPS = -1;//Pull into core
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
	//displacement of model in viewport space
	int displacementX = 0;
	int displacementY = 0;
	double zoomLevel = 1.0f;
};

#endif