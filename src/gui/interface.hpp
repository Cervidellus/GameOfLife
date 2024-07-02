#ifndef GAMEOFLIFE_INTERFACE_HPP
#define GAMEOFLIFE_INTERFACE_HPP

//I'll be renaming this mainWindowMenu or something like that.

#include <presets\modelpresets.hpp>

#include <functional>
#include <memory>
#include <SDL2/SDL_rect.h>

//class ModelPresets::ModelPresetName;
struct SDL_Window;
struct SDL_Renderer;

class Interface {
	public:
		Interface() {};
		~Interface();

		//bool init(
		//	SDL_Window* window, 
		//	SDL_Renderer* renderer
		//);

		//I should make separate update and render functions.
		//I have some kind of callback for when inividual parameters are changed..

		//render should just take a modelParameters reference

		//void draw(
		//	SDL_Renderer* renderer,
		//	bool& modelRunning,
		//	int& desiredModelFPS,
		//	ModelParameters& modelParameters,
		//	const int measuredModelFPS = 0
		//	);

		void startDraw(
			bool& modelRunning,
			int& desiredModelFPS,
			const int measuredModelFPS = 0
		);

		void endDraw(SDL_Renderer* renderer);

		//Mouse related events need to know if the mouse cursor overlaps the overlay. 
		bool isPointInOverlay(int x, int y);

		//Can be used to determine if ImGui frame is being interacted with.
		bool isAnyItemActive();

	private:
		bool isInitialized_ = false;
		SDL_Rect imGuiRect_{0, 0, 0, 0};

};

#endif //GAMEOFLIFE_INTERFACE_HPP