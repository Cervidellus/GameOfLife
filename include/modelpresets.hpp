#ifndef GAMEOFLIFE_MODELPRESETS_H
#define GAMEOFLIFE_MODELPRESETS_H

#include <modelparameters.hpp>

#include <string_view>

//Preset parameters for models.
//In order to add a new model, add a new ModelPresetName, a new getModelPresetStringView, and a new getModelPresetParameters
//If this gets to unwieldly, we can have the parmeters in a separate file.
//Buttons for the presets will be generated in the GUI using the ModelPresetName enum and the getModelPresetStringView function.

namespace ModelPresets {

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
