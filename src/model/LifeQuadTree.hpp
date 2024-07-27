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

		//Coordinate of northWest
		LifeQuadTree::Point origin;

		//Flags that can be set are listed in the enum NodeFlagNames
		NodeFlags flags = 0;

		//bool isRoot = false;
		//bool isLeaf = false;
		////I might switch to flags for all of these bools.. that way I have just a single int.
		//bool alive = false; //Will be true if ANY of the cells in the bounding box are alive. 
		////a flag for an empty cell that has a live neighbor to make sure it gets checked next round.
		////When I set a pixel to true, I'll also set the neighbors to checkNext.
		////If a dead cell is then checked and found to have no neighbors, I set it back to false.
		//bool checkNext = false;

		

		//value for display. 8 bit, will be translated to a color map. 
		uint8_t colorValue = 0;
		//Maybe these should be weakPtr so that if the drawing thing keeps it it doesn't go away? 
		//WOuld that work? So the actual owner would be the data structure for drawing, which will only delete if it goes to zero... 
		//I think though we really want to set it to nullptr 
		//Or maybe I should have hte drawing tree also be a quadtree? And have a separate vector of alive nodes that gets iterated over? 
		std::shared_ptr<Node> parent = nullptr;

		std::shared_ptr<Node> northWest = nullptr;
		std::shared_ptr<Node> northEast = nullptr;
		std::shared_ptr<Node> southEast = nullptr;
		std::shared_ptr<Node> southWest = nullptr;
		const std::array<std::shared_ptr<Node>, 4> children = { northWest , northEast, southEast, southWest };

		bool isEmpty(){
			for (auto quadrant : children) if (quadrant) return false;
			return true;
		}

		bool getFlag(NodeFlagNames flag) { return flags.test(flag); }
		void setFlag(NodeFlagNames flag, bool value) { value ? flags.set(flag) : flags.reset(flag); }

		BoundingBox getBoundingBox()
		{
			//int displacement = 1;
			//for (int i = 1; i <= scale; ++i) displacement = displacement * 2;//I think this works for factorial? Need to check to make sure I get right values.
			int displacement = childDisplacement();
			return BoundingBox
			{
				origin.x ,
				origin.x + displacement,
				origin.y,
				origin.y + displacement
			};
		}

		//The coordinate distance between children;
		int childDisplacement() {
			int displacement = 1;
			for (int i = 1; i <= scale; ++i) displacement = displacement * 2;
			return displacement;
		}
	};

	class Tree
	{
	public:
		Tree();

		std::shared_ptr<LifeQuadTree::Node> rootNode = std::make_unique<LifeQuadTree::Node>();
		void setLeaf(LifeQuadTree::Point point, bool alive = true);
		//getNeighbors(LifeQuadTree::Point point);

		//If it is the first point, call this to create a root node that will include the point.
		//Expand scale by adding root nodes until the root node bounding box includes the point
		void expandTreeScaleTowardsPoint(Point point);


	};
}


#endif // LIFE_QUAD_TREE_H