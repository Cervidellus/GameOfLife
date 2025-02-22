#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL

#include "modelparameters.hpp"

#include <SDL3/SDL_events.h>

struct SDL_Renderer;

class AbstractModel {
public:
	//AbstractModel() = default;
	//~AbstractModel() = default;

	//Expected to set the viewport and initialize the initial state of the model.
	//I might be able to not force this to be reimplemented. 
	virtual void initialize(const SDL_Rect& viewPort) = 0;
	
	virtual void setViewPort(const SDL_Rect& viewPort) { viewPort_ = viewPort; }

	virtual void update() = 0;

	virtual void draw(SDL_Renderer* renderer) = 0;

	virtual void drawImGuiWidgets(const bool& isModelRunning) = 0;

	virtual void handleSDLEvent(const SDL_Event& event){};

protected:
	SDL_Rect viewPort_ = SDL_Rect(0,0,0,0);
};

#endif //ABSTRACT_MODEL