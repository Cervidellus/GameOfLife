#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include "abstract_model.hpp"
#include "ColorMapper.hpp"


#include <vector>
#include <memory>

//Next:: make and sdl texture backbuffer system. Only modify the buffer if there has been a change.
struct SDL_Texture;

class CpuModel : public AbstractModel 
{
public:
	CpuModel();
	~CpuModel() = default;

	void initialize(const SDL_Rect& viewport) override;

	void update() override;

	void handleSDLEvent(const SDL_Event& event) override;

	void draw(SDL_Renderer* renderer) override;

	void drawImGuiWidgets(const bool& isModelRunning) override;

	void setParameters(const ModelParameters& modelParameters);
	ModelParameters getParameters();

	void setViewPort(const SDL_Rect& viewPort) override;

	void generateModel(const ModelParameters& modelParameters);

private:
	
	//Take a stream representing the RLE encoded model and populate board.
	void populateFromRLE_(std::istream& modelStream);
	//Load an RLE file and populate the board. Intended as a callback sent to gui.
	void loadRLE_(const std::string& filePath);
	//Convert and RLE string to a stream and call populateFromRLE_
	void populateFromRLEString_(const std::string& rleString);
	void resizeGrid_();
	void clearGrid_();
	//Whenever the model size is changed, the backbuffer texture must be reinitialized.
	void initBackbuffer_(SDL_Renderer* renderer);

	void recalcDrawRange_();

private:
	const SDL_PixelFormat pixelFormat_ = SDL_PIXELFORMAT_RGB565;

	//primary storage of the model
	std::vector<std::vector<uint8_t>> currentGrid_;
	//used to hold state while we count neighbors to update model
	std::vector<std::vector<uint8_t>> previousGrid_;
	//Used to create the color fading effect in visualization.
	std::vector<std::vector<uint8_t>> colorGrid_;

	//Because the SDL_Texture type is obfuscated and requires an SDL deleter, 
	//we need a template that can accept that deleter.
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> gridBackBuffer_;
	
	ModelParameters activeModelParams_{
		true,
		1000,
		1000
	};

	ColorMapper colorMapper_;
	const int aliveValue_ = 255;
	const int deadValue_ = 0;
	float dualColorAliveColor_[3] = { 1.0, 1.0, 0 };
	float dualColorDeadColor_[3] = { 0.0, 0.0, 1.0 };
	int deadValueDecrement_ = 10;//how fast does teh dead value decrement

	//If the zoom level or displacement changes, recalculates the draw range.
	bool drawRangeRecalcNeeded_ = true;
	//On first pass or on resized model, backbuffer needs reinitialized.
	bool initBackbufferRequired_ = true;

	SDL_Rect drawRange_;
	SDL_FRect destinationRect_;

	int screenSpaceDisplacementX_ = 0;
	int screenSpaceDisplacementY_ = 0;

	const SDL_PixelFormatDetails* formatDetails_ = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGB565);

	const double MAX_ZOOM = 100.0;
	const double MIN_ZOOM = 1.0;

	//for handling ImGui RLE user input
	std::string inputString_ = "";
};

#endif // CPU_MODEL_H
