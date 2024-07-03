#ifndef GAMEOFLIFE_SDL_MANAGER_HPP
#define GAMEOFLIFE_SDL_MANAGER_HPP

//Handles the intialization and shutdown of SDL.

class SDLManager {
public:
	SDLManager();
	~SDLManager();

	void shutdown();

	bool isInitialized() const { return isInitialized_; }

private:
	bool initialize_();

	bool isInitialized_ = false;
};

#endif //GAMEOFLIFE_SDL_MANAGER_HPP