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
	std::vector<std::pair <int, int>> aliveCells = {};//I guess this is ok as the presets will be sparse. Makes it easier to define.
};

namespace ModelPresets {
	const ModelParameters random = ModelParameters();

	const ModelParameters swiss_cheese = ModelParameters(
		true,
		15,
		-1,
		-1,
		0.9f,
		5,
		8,
		1
	);

	const ModelParameters decomposition = ModelParameters(
		true,
		40,
		-1,
		-1,
		0.9f,
		5,
		8,
		3
	);

	const ModelParameters blinker = ModelParameters(
		false,
		-1,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		3,
		3,
		{{2, 1}, {2, 2}, {2, 3}}
	);
}

#endif