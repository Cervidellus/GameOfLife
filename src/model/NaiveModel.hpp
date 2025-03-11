#ifndef NAIVE_MODEL_H
#define NAIVE_MODEL_H

#include "abstract_model.hpp"

#include <vector>

class NaiveModel : public AbstractModel 
{
public:
	NaiveModel();
	~NaiveModel() = default;

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

private:
	//primary storage of the model
	std::vector<std::vector<uint8_t>> currentGrid_;
	//used to hold state while we count neighbors to update model
	std::vector<std::vector<uint8_t>> previousGrid_;
};

#endif // NAIVE_MODEL_H
