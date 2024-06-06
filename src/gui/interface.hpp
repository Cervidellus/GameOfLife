#ifndef GAMEOFLIFE_INTERFACE_HPP
#define GAMEOFLIFE_INTERFACE_HPP

//I'll be renaming this mainWindowMenu or something like that.

#include <presets\modelpresets.hpp>

#include <functional>
#include <memory>

//class ModelPresets::ModelPresetName;
struct SDL_Window;
struct SDL_Renderer;
class SDL_Texture;

class Interface {
	public:
		Interface();
		~Interface();

		bool init(
			SDL_Window* window, 
			SDL_Renderer* renderer,
			std::function<void(ModelParameters presetParameters)> presetCallback,
			std::function<SDL_Texture*()> getWindowTextureCallback);

		//I should make separate update and render functions.
		//I have some kind of callback for when inividual parameters are changed..

		//render should just take a modelParameters reference

		void draw(
			ModelParameters& modelParameters,
			//int& modelFPS, 
			const int measuredModelFPS = 0
			//bool& modelRunning,
			//float& fillFactor,
			//int& modelWidth,
			//int& modelHeight,
			//int& rule1,
			//int& rule3,
			//int& rule4
			);

	private:
		bool isInitialized_ = false;
		std::unique_ptr<std::function<void(ModelParameters presetParameters)>> presetCallback_;
		std::unique_ptr<std::function<SDL_Texture*()>> getWindowTextureCallback_;
};

#endif //GAMEOFLIFE_INTERFACE_HPP