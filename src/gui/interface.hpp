#ifndef GAMEOFLIFE_INTERFACE_HPP
#define GAMEOFLIFE_INTERFACE_HPP

//I'll be renaming this mainWindowMenu or something like that.

#include <presets\modelpresets.hpp>

#include <functional>
#include <memory>
#include <SDL3/SDL_rect.h>

//class ModelPresets::ModelPresetName;
struct SDL_Window;
struct SDL_Renderer;

class Interface {
	public:
		Interface() {};
		~Interface();

		void startDraw(
			bool& modelRunning,
			const char* const* modelNames,
			int& selectedModel,
			int& desiredModelFPS,
			const int measuredModelFPS = 0
		);

		void endDraw(SDL_Renderer* renderer);

	private:
		SDL_Rect imGuiRect_{0, 0, 0, 0};
};

#endif //GAMEOFLIFE_INTERFACE_HPP