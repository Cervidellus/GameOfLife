#ifndef ABSTRACT_MODEL
#define ABSTRACT_MODEL

#include "ColorMapper.hpp"
#include "modelparameters.hpp"

#include <memory>

#include <SDL3/SDL_events.h>

struct SDL_Renderer;
struct SDL_Texture;

class AbstractModel {
public:
	AbstractModel();
	//~AbstractModel() = default;

	//Expected to set the viewport and initialize the initial state of the model.
	virtual void initialize(const SDL_Rect& viewPort);

	virtual void generateModel(const ModelParameters& params);
	
	virtual void setViewPort(const SDL_Rect& viewPort);

	virtual void setCell(int x, int y, uint8_t value) = 0;

	virtual uint8_t getCell(int x, int y) = 0;

	virtual size_t dataColumns() = 0;
	
	virtual size_t dataRows() = 0;

	virtual void update() = 0;

	virtual void draw(SDL_Renderer* renderer) = 0;

	virtual void drawImGuiWidgets(const bool& isModelRunning) = 0;

	virtual void handleSDLEvent(const SDL_Event& event){};

	void setParameters(const ModelParameters& modelParameters);

	ModelParameters getParameters();

protected:

	//Take a stream representing the RLE encoded model and populate board.
	void populateFromRLE_(std::istream& modelStream);
	////Load an RLE file and populate the board. Intended as a callback sent to gui.
	void loadRLE_(const std::string& filePath);
	////Convert and RLE string to a stream and call populateFromRLE_
	void populateFromRLEString_(const std::string& rleString);

	virtual void resizeGrid_() = 0;
	virtual void clearGrid_() = 0;
	////Whenever the model size is changed, the backbuffer texture must be reinitialized.
	void initBackbuffer_(SDL_Renderer* renderer);
	//void updateCell_(int col, int row, int previousValue, int neighborCount);

protected:
	
	virtual void recalcDrawRange_();

	ModelParameters activeModelParams_{
	true,
	1000,
	1000
	};

	SDL_Rect viewPort_ = SDL_Rect(0,0,0,0);
	SDL_Rect drawRange_;
	bool drawRangeRecalcNeeded_ = true;
	SDL_FRect destinationRect_;
	//Reflects where top left corner of the model will start to display.
	int screenSpaceDisplacementX_ = 0;
	int screenSpaceDisplacementY_ = 0;
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> gridBackBuffer_;

	ColorMapper colorMapper_;
	const int aliveValue_ = 255;
	const int deadValue_ = 0;
	float dualColorAliveColor_[3] = { 1.0, 1.0, 0 };
	float dualColorDeadColor_[3] = { 0.0, 0.0, 1.0 };
	int deadValueDecrement_ = 10;//how fast does teh dead value decrement
	//On first pass or on resized model, backbuffer needs reinitialized.
	bool initBackbufferRequired_ = true;

	const SDL_PixelFormatDetails* formatDetails_ = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGB565);

	const double MAX_ZOOM = 100.0;
	const double MIN_ZOOM = 1.0;

	//for handling ImGui RLE user input
	std::string inputString_ = "";
};

#endif //ABSTRACT_MODEL