#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include "abstract_model.hpp"
#include "modelparameters.hpp"

#include <vector>
#include <../submodules/tinycolormap/include/tinycolormap.hpp>



class CpuModel : public AbstractModel 
{
public:
	CpuModel() = default;
	~CpuModel() = default;

	void initialize() override;

	void update() override;

	void handleSDLEvent(const SDL_Event& event) override;

	void draw(
		SDL_Renderer* renderer, 
		const int posX, 
		const int posY, 
		const int width, 
		const int height) override;

	void drawImGuiWidgets(const bool& isModelRunning) override;

	void setParameters(const ModelParameters& modelParameters);
	ModelParameters getParameters();

	//void setDrawStrategy(SquareGridDrawStrategy& strategy); When I want to implement different drawing strategies, I can do this.

private:
	//I should have a squareGridGenerator, but what about different underlying storage types?
	//I'll put it here for now.
	//void handleGenerateModelRequest_(const ModelParameters& params);
	void generateModel_(const ModelParameters& modelParameters);
	void populateFromRLEString_(std::string model, const int startRow = 0, const int startColumn = 0);
	void resizeGrid_();
	void clearGrid_();

	struct GridDrawRange
	{
		int rowBegin = 0;
		int rowEnd = 1;
		int columnBegin = 0;
		int columnEnd = 1;
	};

	enum class DrawStrategy
	{
		SingleColor,
		Decay
	};

	GridDrawRange getDrawRange_(int rowCount, int columnCount);
	void drawDecay_(SDL_Renderer* renderer, const GridDrawRange& drawRange);

private:
	std::vector<std::vector<uint8_t>> grid_; //I use an 8 but int so I can represent some other info for visualization.
	
	ModelParameters activeModelParams_{
		true,
		400,
		400
	};
	//My intention is that 0 is dead, 255 is alive, and in between numbers can either be the amount of time alive or the amount of time dead.
	//valuses for aliveValue_ and deadValue_ can be changed for different visualization strategies.
	int aliveValue_ = 255;
	int deadValue_ = 0;
	float singleDrawColor_[3] = { 1.0, 1.0, 0 };
	int deadValueDecrement_ = 5;//how fast does teh dead value decrement
	DrawStrategy drawStrategy_ = DrawStrategy::Decay;
	tinycolormap::ColormapType colorMapType_ = tinycolormap::ColormapType::Inferno;
	//have in the draw function check the strategy, and hten call an appropriate function. 

	const double MAX_ZOOM = 100.0;
	const double MIN_ZOOM = 1.0;

};

#endif // CPU_MODEL_H
