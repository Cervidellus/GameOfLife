#ifndef LIFEQUADTREEMODEL_HPP
#define LIFEQUADTREEMODEL_HPP

#include "abstract_model.hpp"
#include "LifeQuadTree.hpp"

class LifeQuadTreeModel : public AbstractModel
{
	public:
	LifeQuadTreeModel() = default;
	~LifeQuadTreeModel() = default;

	void initialize(const SDL_Rect& viewport) override;

	void update() override;

	void handleSDLEvent(const SDL_Event& event) override;

	void draw(SDL_Renderer* renderer) override;

	void drawImGuiWidgets(const bool& isModelRunning) override;

	//void setParameters(const ModelParameters& modelParameters);
	//ModelParameters getParameters();

private:
	LifeQuadTree::Tree tree_;
	ModelParameters activeModelParams_{
	true,
	400,
	400
	};

	void generateModel_(const ModelParameters& modelParameters);
	//void populateFromRLE_(std::istream& modelStream);
	//void resizeGrid_();
	//void clearGrid_();

	struct GridDrawRange
	{
		int rowBegin = 0;
		int rowEnd = 1;
		int columnBegin = 0;
		int columnEnd = 1;
	};

	GridDrawRange getDrawRange_(const int width, const int height);

};

#endif //LIFEQUADTREEMODEL_HPP