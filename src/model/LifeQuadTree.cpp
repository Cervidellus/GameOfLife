#include "LifeQuadTree.hpp"

#include <algorithm>
#include <cmath>

bool LifeQuadTree::isInBoundingBox(Point point, BoundingBox box)
{
	return (point.x >= box.xMin && point.x <= box.xMax && point.y >= box.yMin && point.y <= box.yMax);
}

		//We can get the hierarchy level from the root node.
LifeQuadTree::Tree::Tree()
{
	//rootNode->flags.set(NodeFlagNames::IsRoot);
}



void LifeQuadTree::Tree::expandTreeScaleTowardsPoint(Point point)
{
	//if it is the first point, move the root to the origin point.
	if (rootNode->isEmpty())
	{
		rootNode->scale = 1;
		rootNode->origin = point;
		return;
	}

	// 	//Create a new Node with the current root node as a child in the appropriate place. (e.g. nw, ne, sw, se etc). 
	auto oldBoundingBox = rootNode->getBoundingBox();
	auto newRoot = std::make_shared<Node>();
	rootNode->parent = newRoot;

	//set the origin correctly for newNode.
	//setRootNode to the right place in newNode;
	// I would do ternaries if I didn't also have to put it in the right child node.
	//newRoot->origin.x = (point.x >= oldBoundingBox.xMax) ? rootNode->origin.x : rootNode->origin.x - newRoot->childDisplacement();
	//newRoot->origin.y = (point.y >= oldBoundingBox.yMax) ? rootNode->origin.y : rootNode->origin.y - newRoot->childDisplacement();
	if (point.x >= oldBoundingBox.xMax)//Root needs to be west so we expand east
	{
		if (point.y >= oldBoundingBox.yMax)//root needs to be north so we expand south
		{
			newRoot->origin = rootNode->origin;
			newRoot->northWest = rootNode;
		}
		else //root needs to be south so we expand north
		{
			newRoot->origin = Point{ rootNode->origin.x, rootNode->origin.y - newRoot->childDisplacement() };
			newRoot->southWest = rootNode;
		}
	}
	else //Root needs to be east so we expand west
	{
		if (point.y >= oldBoundingBox.yMax)//Root needs to be north so we expand south
		{
			newRoot->origin = Point{ rootNode->origin.x - newRoot->childDisplacement(), rootNode->origin.y };
			newRoot->northEast = rootNode;
		}
		else //need to be south so we expand north
		{
			newRoot->origin = Point{ rootNode->origin.x - newRoot->childDisplacement(), rootNode->origin.y - newRoot->childDisplacement() };
			newRoot->southEast = rootNode;
		}
	}
	rootNode = newRoot;
}

void LifeQuadTree::Tree::setLeaf(LifeQuadTree::Point point, bool alive)
{
	//First try and find the correct parent starting from root.
	
	

	//If correct parent could not be found, expand the tree.
	//Check to see if it is in bounding box of tree root. If not, expand the tree.
	while(!isInBoundingBox(point, rootNode->getBoundingBox())) expandTreeScaleTowardsPoint(point);

	auto currentNode = rootNode;
	while (currentNode->scale >= 0)
	{
		//traverse down until you get to a leaf node. If at any point you reach a nullpointer, create that node.
	}
}