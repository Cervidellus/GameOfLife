#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include "abstract_model.hpp"
#include "ColorMapper.hpp"
//#include "modelparameters.hpp"

#include <vector>
//#include <../submodules/tinycolormap/include/tinycolormap.hpp>

class CpuModel : public AbstractModel 
{
public:
	CpuModel() = default;
	~CpuModel() = default;

	void initialize(const SDL_Rect& viewport) override;

	void update() override;

	void handleSDLEvent(const SDL_Event& event) override;

	void draw(SDL_Renderer* renderer) override;

	void drawImGuiWidgets(const bool& isModelRunning) override;

	void setParameters(const ModelParameters& modelParameters);
	ModelParameters getParameters();

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

	struct GridDrawRange
	{
		int rowBegin = 0;
		int rowEnd = 1;
		int columnBegin = 0;
		int columnEnd = 1;
	};

	GridDrawRange getDrawRange_
	(
		const int& width,
		const int& height,
		const int& displacementX, 
		const int& displacementY
	);

private:
	std::vector<std::vector<uint8_t>> grid_; //I use an 8 but int so I can represent some other info for visualization.
	
	ModelParameters activeModelParams_{
		true,
		400,
		400
	};

	ColorMapper colorMapper_;
	const int aliveValue_ = 255;
	const int deadValue_ = 0;
	float dualColorAliveColor_[3] = { 1.0, 1.0, 0 };
	float dualColorDeadColor_[3] = { 0.0, 0.0, 1.0 };
	int deadValueDecrement_ = 10;//how fast does teh dead value decrement

	//If the zoom level or displacement changes, recalculates the draw range.
	//set to true so that it calculates on first draw.
	bool recalcDrawRange_ = true;
	GridDrawRange drawRange_;

	//int selectedColorMapIndex_ = 0;

	const double MAX_ZOOM = 100.0;
	const double MIN_ZOOM = 1.0;

	//for handling ImGui RLE user input
	std::string inputString_ = "";
};

#endif // CPU_MODEL_H
