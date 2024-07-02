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
	AbstractModel() = default;
	~AbstractModel() = default;

	//I think eventually the model parameters will not be in abstract class. I think I need it here now just to get it going.
	ModelParameters modelParameters;

	//Very likely I can get rid of this.
	virtual void initialize() = 0;
	//virtual void processEvents(const SDL_Event& event) = 0;
	virtual void update() = 0;	

	//I could also have a drawing strategy pattern.. so if I want to do squares, or circles, or have things run into each other. 
	//I don't think the different drawing strategies belong in the abstract class though.
	virtual void draw(SDL_Renderer* renderer, const int posX, const int posY, const int width, const int height) = 0;
	//Does not include begin or end frame, so widgets can slot into an existing frame.
	virtual void drawImGuiWidgets(const bool isModelRunning) = 0;

	virtual void handleSDLEvent(
		const SDL_Event& event, 
		const int& mousePosX,
		const int& mousePosY,
		const int& mouseButtonState,
		const bool& isCursorInOverlay
		){};

	//Scale from 0 to 1
	void setRenderScale(double scale)
	{
		if (scale > 0 && scale <= 1) renderScale_ = scale;
	}

	//When scale is less than 1, move the viewport from the center. 
	// Scales from from -1 to 1, with default being center. 
	void setRenderRelativePosition(double x, double y)
	{
		if (x >= -1 && x <= 1) relativePositionX_ = x;
		if (y >= -1 && y <= 1) relativePositionY_ = y;
	}
	
	//Increase or decrease the deviation from center by a percentage from center
	void incrementRelativePosition(double x, double y)
	{
		relativePositionX_ += x;
		relativePositionY_ += y;

		if (relativePositionX_ > 1) relativePositionX_ = 1;
		else if (relativePositionX_ < -1) relativePositionX_ = -1;

		if (relativePositionY_ > 1) relativePositionY_ = 1;
		else if (relativePositionY_ < -1) relativePositionY_ = -1;
	}

	//SHould I also add a draw that specifies a viewport? That way I only have to draw what is visible.

	//I could have something that provides an IMGUI interface for options, and connects callbacks to set parameters

	//I'll add that after I get it working though. 

	//I could also have an event handler

	//

private:
	
	double renderScale_ = 1.0;
	double relativePositionX_ = 0.0;
	double relativePositionY_ = 0.0;
	//I need an underlying stora ge type... should I have a vector of vectors or just a vector, and length and width params? 
};
