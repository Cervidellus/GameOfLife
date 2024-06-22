#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include "abstract_model.hpp"
#include "modelparameters.hpp"

#include <vector>

class CpuModel : public AbstractModel {

public:
	CpuModel() = default;
	~CpuModel() = default;

	void initialize() override;
	void update() override;
	void draw(SDL_Texture* texture) override;
	void setParameters(const ModelParameters& modelParameters);

	//void setModelParameters(ModelParameters modelParameters);

	//void setDrawStrategy(SquareGridDrawStrategy& strategy); When I want to implement different drawing strategies, I can do this.

private:
	//I should have a squareGridGenerator, but what about different underlying storage types?
	//I'll put it here for now.
	//void handleGenerateModelRequest_(const ModelParameters& params);
	void generateModel_(const ModelParameters& modelParameters);
	void populateFromRLEString_(std::string model, const int startRow = 0, const int startColumn = 0);
	void resizeGrid_();
	void clearGrid_();


private:
	std::vector<std::vector<uint8_t>> grid_; //I use an 8 but int so I can represent some other info for visualization.
	//My intention is that 0 is dead, 255 is alive, and in between numbers can either be the amount of time alive or hte amount of time dead.
	ModelParameters activeModelParams_;
	//valuse sfor aliveValue_ and deadValue_ can be changed for different visualization strategies.
	int aliveValue_ = 255;
	int deadValue_ = 0;
}

#endif // CPU_MODEL_H
