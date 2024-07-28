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

		//Point& operator=(const Point& point) {
		//	x = point.x;
		//	y = point.y;
		//	return *this;
		//}

		//Point operator+(const Point& point) const {
		//	return { x + point.x, y + point.y };
		//}

		bool operator==(const Point& point) const
		{
			return (x == point.x && y == point.y);
		}

		//bool operator==(const Names& rhs) const {
		//	return this->fname == rhs.lname;
		//}
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
		//We actually don't really have to store this as it can be deduced from the tree structure.
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


		//This is just wrong.... 
		BoundingBox getBoundingBox()
		{
			//int displacement = 1;
			//for (int i = 1; i <= scale; ++i) displacement = displacement * 2;//I think this works for factorial? Need to check to make sure I get right values.
			int displacement = childDisplacement();
			return BoundingBox
			{
				origin.x ,
				2 * displacement + origin.x -1,
				origin.y,
				2 * displacement + origin.y -1
			};
		}

		//The coordinate distance between children;
		//I can get this from boundinghbox. 
		int childDisplacement() {
			int displacement = 1;//scale 1 we want 1
			//scale 2 we want 2.. but it returns 4? 
			
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
		//getNeighbors(LifeQuadTree::Point point);

		//If it is the first point, call this to create a root node that will include the point.
		//Expand scale by adding root nodes until the root node bounding box includes the point

		std::shared_ptr<LifeQuadTree::Node> getChildNodeEnclosingPoint(
			std::shared_ptr<LifeQuadTree::Node> parent,
			LifeQuadTree::Point point
		);

		void expandTreeScaleTowardsPoint(Point point);


	};
}


#endif // LIFE_QUAD_TREE_H