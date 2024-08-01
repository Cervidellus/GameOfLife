#ifndef GAMEOFLIFE_MODEL_PARAMS
#define GAMEOFLIFE_MODEL_PARAMS

#include <string>
#include <vector>
//For most values, negative values will be ignored
struct ModelParameters {
	bool random = true;
	int modelWidth = -1;
	int modelHeight = -1;
	float fillFactor = 0.2f;
	int rule1 = 2; //Minimum number of alive neighbors for an alive cell to stay alive.
	int rule3 = 3; //Maximum number of alive neighbors for an live cell to stay alive.
	int rule4 = 3;//number of alive neighbors for a dead cell to become alive. 
	int minWidth = 10;
	int minHeight = 10;
	std::string runLengthEncoding = "";
	//displacement of model in viewport space
	int displacementX = 0;
	int displacementY = 0;
	double zoomLevel = 1.0f;
};

#endif