#ifndef GAMEOFLIFE_MODELPRESETS_H
#define GAMEOFLIFE_MODELPRESETS_H

#include <modelparameters.hpp>

#include <string_view>

class SDL_Surface;

namespace ModelPresets {

	//SDL_Surface* generateModelPresetSurface(const ModelParameters& modelParameters); 

	enum class ModelPresetName {
		random,
		swiss_cheese,
		decomposition
	};

	constexpr std::string_view getModelPresetStringView(ModelPresetName modelPresetName){
		switch (modelPresetName) {
			case ModelPresetName::random:
				return "random";
			case ModelPresetName::swiss_cheese:
				return "swiss_cheese";
			case ModelPresetName::decomposition:
				return "decomposition";
			default:
				return "unknown";
		}
	};

	const ModelParameters randomParams = ModelParameters();

	const ModelParameters swissCheeseParams = ModelParameters(
		true,
		15,
		-1,
		-1,
		0.9f,
		5,
		8,
		1
	);

	const ModelParameters decompositionParams = ModelParameters(
		true,
		40,
		-1,
		-1,
		0.9f,
		5,
		8,
		3
	);

	const ModelParameters blinkerParams = ModelParameters(
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
		{ {2, 1}, {2, 2}, {2, 3} }
	);
}

#endif //GAMEOFLIFE_MODELPRESETS_H
