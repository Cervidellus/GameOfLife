#ifndef WIDGET_FUNCTIONS_HPP
#define WIDGET_FUNCTIONS_HPP

#include "../model/modelparameters.hpp"
#include "../model/ColorMapper.hpp"

#include <functional>

//Functions for drawing groups of ImGui widgets.
//generateModelCallback is a function that will generate a new model with the given parameters.
//loadPresetFileCallback returns a std::string with the path of a file to load.
// loadRLEStringCallback and RLEString are used in a popup window to enter an RLE string.
//loadRLEStringCallback returns void when the user is finished entering an RLE string.
//RLEString is a string is modified whenever the user enters data,
//and is intended to be used after loadRLEStringCallback returns.

namespace WidgetFunctions
{
	void drawGOLRulesHeader(
		ModelParameters& modelParameters,
		std::function<void(const ModelParameters&)> generateModelCallback,
		const bool modelRunning
	);

    void drawVisualizationHeader(
		ModelParameters& modelParameters,
		ColorMapper& colorMapper,
		int& colorDecayRate,
		bool& displacementChanged,
		const bool modelRunning
	);

	void drawPresetsHeader(
		ModelParameters& modelParameters,
		std::function<void(const ModelParameters&)> generateModelCallback,
		std::function<void(std::string)> loadPresetFileCallback,
		std::function<void()> loadRLEStringCallback,
		std::string& RLEString,
		const bool modelRunning
	);
}

#endif //WIDGET_FUNCTIONS_HPP