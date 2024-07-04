#ifndef GAMEOFLIFE_MODELPRESETS_H
#define GAMEOFLIFE_MODELPRESETS_H

#include <model\modelparameters.hpp>

#include <string_view>

//Preset parameters for models.
//In order to add a new model, add a new ModelPresetName, a new getModelPresetStringView, and a new getModelPresetParameters
//If this gets to unwieldly, we can have the parmeters in a separate file.
//Buttons for the presets will be generated in the GUI using the ModelPresetName enum and the getModelPresetStringView function.

namespace ModelPresets {

	const ModelParameters randomParams = ModelParameters();

	const ModelParameters swissCheeseParams = ModelParameters(
		true,
		-1,
		-1,
		0.9f,
		5,
		8,
		1
	);

	const ModelParameters decompositionParams = ModelParameters(
		true,
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
		2,
		3,
		3,
		3,
		3,
		{},
		"3o!"
	);

	const ModelParameters lightweightSpaceshipParams = ModelParameters(
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		40,
		40,
		{},
		"bo2bo$o4b$o3bo$4o!"
	);

	const ModelParameters blockerParams = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		110,
		110,
		{},
		"6bobob$5bo4b$2o2bo4bo$2obo2bob2o$4b2o!"
	};

	const ModelParameters nihoniumParams = {
	false,
	-1,
	-1,
	-1,
	2,
	3,
	3,
	110,
	110,
	{},
	"42b2o$42b2o5b2o$49b2o$13b2o$14bo$13bo33b2o$13b2o32b2o$53b2o$2o51b2o$b"
	"o$bob2o$2bo$14b2o$4bobo8bo$5bo7b3o$6bo46bo$3b4o45bobo$2bo49bobo$2b2ob"
	"2o44b2ob2o$3bobo49bo$3bobo45b4o$4bo46bo$42b3o7bo$42bo8bobo$42b2o$55bo"
	"$53b2obo$56bo$3b2o51b2o$3b2o$9b2o32b2o$9b2o33bo$43bo$43b2o$7b2o$7b2o5b"
	"2o$14b2o!"
	};

const ModelParameters gabrielsPOneThirtyEightParams = {
	false,
	-1,
	-1,
	-1,
	2,
	3,
	3,
	33,
	33,
	{},
	"8b3o10b$7bo2bo10b$7bo2bo10b$7b2o12b4$17b3ob$17bo2bo$20bo$3o15b3o$o20b$"
	"o2bo17b$b3o17b4$12b2o7b$10bo2bo7b$10bo2bo7b$10b3o!"
	};
}

#endif //GAMEOFLIFE_MODELPRESETS_H
