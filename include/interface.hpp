#ifndef GAMEOFLIFE_INTERFACE_HPP
#define GAMEOFLIFE_INTERFACE_HPP

#include <functional>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

//template <typename Callback>

enum class ModelPreset {
	random,
	swiss_cheese,
	decomposition
};

class Interface {
	public:
		Interface();
		~Interface();

		bool init(
			SDL_Window* window, 
			SDL_Renderer* renderer,
			std::function<void(ModelPreset preset)> generateModelButtonCallback);

		void render(
			int& modelFPS, 
			const int measuredModelFPS,
			bool& modelRunning,
			float& fillFactor,
			int& modelWidth,
			int& modelHeight,
			int& rule1,
			int& rule3,
			int& rule4
			);

	private:
		std::unique_ptr<std::function<void(ModelPreset preset)>> generateModelCallback_;
};

#endif //GAMEOFLIFE_INTERFACE_HPP