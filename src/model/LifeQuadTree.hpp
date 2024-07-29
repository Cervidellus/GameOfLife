#ifndef LIFE_QUAD_TREE_H
#define LIFE_QUAD_TREE_H

#include <array>
#include <bitset>
#include <memory>

//I could also do a version where I stored chunks of cells in a single int and do bit manipulation to deal with them.
//But I want to do a simpler version first.


namespace LifeQuadTree
{
	typedef std::bitset<8>  NodeFlags;

	struct Point
	{
		int x = 0;
		int y = 0;

		bool operator==(const Point& point) const
		{
			return (x == point.x && y == point.y);
		}
	};

	struct BoundingBox
	{
		int xMin;
		int xMax;
		int yMin;
		int yMax;
	};

	bool isInBoundingBox(Point point, BoundingBox box);

	enum NodeFlagNames
	{
		//IsRoot = 0,// can be inferred by having no parent.
		//IsLeaf = 1,//can infer from scale
		IsAlive = 1,
		CheckNext = 2
	};

	struct Node
	{
		//Level in the hierarchy, with 0 being the lowest level with.
		//Scale 1 is where the children are each a single pixel. Leaf nodes (single pixels) have scale 0;
		int scale = 0;
		//Coordinate system with 0,0 being the center of the world. 
		//Bounding box has the origin in the upper left corner.
		//When I draw, I'll have to translate that to the screen space.

		//If it is a LEAF node, the origin is just the point coordinates.

		//We don't have to store this as it can be deduced from the tree structure.
		//For now I am leaving it in to make my life easy..
		LifeQuadTree::Point origin;

		//Flags that can be set are listed in the enum NodeFlagNames
		NodeFlags flags = 0;

		//value for display. 8 bit, will be translated to a color map. 
		uint8_t colorValue = 0;
		//Maybe these should be weakPtr so that if the drawing thing keeps it it doesn't go away? 
		//WOuld that work? So the actual owner would be the data structure for drawing, which will only delete if it goes to zero... 
		//I think though we really want to set it to nullptr 
		//Or maybe I should have hte drawing tree also be a quadtree? And have a separate vector of alive nodes that gets iterated over? 
		std::shared_ptr<Node> parent = nullptr;
		
		//I should just have this in the array... but this allows me to access by name. Leaving it in for now to make my life easier. 
		std::shared_ptr<Node> northWest = nullptr;
		std::shared_ptr<Node> northEast = nullptr;
		std::shared_ptr<Node> southEast = nullptr;
		std::shared_ptr<Node> southWest = nullptr;
		//This needs to be pointers to pointers!
		const std::array<std::shared_ptr<Node>*, 4> children = { &northWest , &northEast, &southEast, &southWest };

		//Return true if all quadrant pointers are empty
		bool isEmpty(){
			for (auto quadrant : children) if (*quadrant) return false;
			return true;
		}

		bool getFlag(NodeFlagNames flag) { return flags.test(flag); }
		void setFlag(NodeFlagNames flag, bool value) { value ? flags.set(flag) : flags.reset(flag); }

		BoundingBox getBoundingBox()
		{
			int displacement = childDisplacement();
			return BoundingBox
			{
				origin.x ,
				2 * displacement + origin.x -1,
				origin.y,
				2 * displacement + origin.y -1
			};
		}

		int childDisplacement() {
			//I could make a simple lookup table to make this run faster. 
			int displacement = 1;
			for (int i = 1; i < scale; i++) displacement = displacement * 2;
			return displacement;
		}
	};

	class Tree
	{
	public:
		Tree();

		std::shared_ptr<LifeQuadTree::Node> rootNode = std::make_unique<LifeQuadTree::Node>(1);
		std::shared_ptr<LifeQuadTree::Node> setLeaf(LifeQuadTree::Point point, bool alive = true);
		//clockwise Moore neighborhood, with northwest being index 0; 
		//std::array<std::shared_ptr<LifeQuadTree::Node>, 8> getMooreNeighborhood(std::shared_ptr<LifeQuadTree::Node> center);

		//If the center does not exist yet, I can call this?
		//std::array<std::shared_ptr<LifeQuadTree::Node>, 8> getMooreNeighborhood(const LifeQuadTree::Point center);

		//returns false if a point does not exist. 
		//bool isPointAlive(LifeQuadTree::Point point);
		
		//std::shared_ptr<LifeQuadTree::Node> getPoint(LifeQuadTree::Point point);

		//Descends tree one level from the tree towards a point.
		std::shared_ptr<LifeQuadTree::Node> descendTowardsPoint(
			std::shared_ptr<LifeQuadTree::Node> parent,
			LifeQuadTree::Point point
		);

		//I can also make an ascend method. But this is just getting the parent. 
		
		//this can be private
		void expandTreeScaleTowardsPoint(Point point);


	};
}


#endif // LIFE_QUAD_TREE_H