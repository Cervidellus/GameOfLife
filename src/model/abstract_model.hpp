//I need an abstract model class that will act as an interface for rendering.
//I can have different implementations underneath

//I'll have a simple CPU square grid representation.Use a vector of vectors for rows and columns
// Maybe I should do a sparse representation? I can build my own data type or use Eigen. 
//I'll implement a square grid with CUDA
//I'll implement a grid with intel's sycl or dpc++. 
//AdaptiveCpp(hipSYCL) from AMD
//Vulkan?

#include "modelparameters.hpp"

#include <SDL2/SDL_events.h>

struct SDL_Renderer;

class AbstractModel {
public:
	//AbstractModel() = default;
	//~AbstractModel() = default;

	//Expected to set the viewport and initialize the initial state of the model.
	//I might be able to not force this to be reimplemented. 
	virtual void initialize(const SDL_Rect& viewPort) = 0;
	
	void setViewPort(const SDL_Rect& viewPort) { viewPort_ = viewPort; }

	virtual void update() = 0;	

	virtual void draw(SDL_Renderer* renderer) = 0;

	virtual void drawImGuiWidgets(const bool& isModelRunning) = 0;

	virtual void handleSDLEvent(const SDL_Event& event){};

protected:
	SDL_Rect viewPort_;
};
