#ifndef LESS_NAIVE_MODEL_H
#define LESS_NAIVE_MODEL_H

#include "abstract_model.hpp"
#include "VectorGrid.hpp"

//Like the Naive model, with a few improvements.
// Stores in a continuous vector for better memory adjacency.
//Small efficiency in calculating neighbors. 

class LessNaiveModel : public AbstractModel
{
public:
	LessNaiveModel();
	~LessNaiveModel() = default;

	void setCell(int x, int y, uint8_t value) override;

	uint8_t getCell(int x, int y) override;

	size_t dataColumns();

	size_t dataRows();

	void update() override;

	void handleSDLEvent(const SDL_Event& event) override;

	void draw(SDL_Renderer* renderer) override;

	void drawImGuiWidgets(const bool& isModelRunning) override;

private:
	void resizeGrid_();
	void clearGrid_();
	void updateCell_(int col, int row, int previousValue, int neighborCount);

private:
	//primary storage of the model
	VectorGrid currentGrid_;
	//used to hold state while we count neighbors to update model
	VectorGrid previousGrid_;
};

#endif // LESS_NAIVE_MODEL_H
