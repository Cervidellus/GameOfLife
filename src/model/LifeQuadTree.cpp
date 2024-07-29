#include "LifeQuadTree.hpp"

#include <algorithm>
#include <iostream>
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
	//if (point == Point{ -5, -44 })
	//{
	//	std::cout << "here is the fail point. Returns wrong child node.\n";
	//	//Root node has scale 1?
	//}
	//if it is the first point, move the root to the origin point.
	if (rootNode->isEmpty())//I could flag this unlikely. Don't want to do it until I have benchmarking in place.
	{
		rootNode->scale = 1;
		rootNode->origin = point;
		return;
	}

	// 	//Create a new Node with the current root node as a child in the appropriate place. (e.g. nw, ne, sw, se etc). 
	//I might not really have to allocate a boundingbox... it is simple math. I can benchmark it later.
	//auto oldBoundingBox = rootNode->getBoundingBox();
	auto newRoot = std::make_shared<Node>();
	newRoot->scale = rootNode->scale + 1;
	rootNode->parent = newRoot;

	//set the origin correctly for newNode.
	//setRootNode to the right place in newNode;
	//I'm expanding in the wrong direction!
	//this is working when we want to expand the root in certain directions, failing in others. 
	if (point.x >= rootNode->origin.x)//origin.x stays same so we expand east
	{
		newRoot->origin.x = rootNode->origin.x;
		if (point.y >= rootNode->origin.y)//origin.y stays the same so we expand south
		{
			newRoot->origin.y = rootNode->origin.y;
			newRoot->northWest = rootNode;
		}
		else //origin.y needs to be south so we expand north
		{
			newRoot->origin.y = rootNode->origin.y - (2 * newRoot->childDisplacement());
			newRoot->southWest = rootNode;
		}
	}
	else //origin.x to be east so we expand west
	{
		newRoot->origin.x = rootNode->origin.x - (2 * newRoot->childDisplacement());
		if (point.y >= rootNode->origin.y)//Root needs to be north so we expand south
		{
			newRoot->origin.y = rootNode->origin.y;
			//newRoot->origin = Point{ rootNode->origin.x - newRoot->childDisplacement(), rootNode->origin.y };
			newRoot->northEast = rootNode;
		}
		else //need to be south so we expand north
		{
			newRoot->origin.y = rootNode->origin.y - (2 * newRoot->childDisplacement());
			//newRoot->origin = Point{ rootNode->origin.x - newRoot->childDisplacement(), rootNode->origin.y - newRoot->childDisplacement() };
			newRoot->southEast = rootNode;
		}
	}
	rootNode = newRoot;
}

std::shared_ptr<LifeQuadTree::Node> LifeQuadTree::Tree::descendTowardsPoint(
	std::shared_ptr<LifeQuadTree::Node> parent,
	LifeQuadTree::Point point)
{
	if (parent->scale == 0) return parent;
	//auto boundingBox = parent->getBoundingBox();
	//if (point == Point{ -5, -44 } )
	//{
	//	std::cout << "here is the fail point. Returns wrong child node.\n";
	//	//Bounding box of root is WAY off....
	//}

	//traverse down until you get to a leaf node. If at any point you reach a nullpointer, create that node.
	//Which child bounding box does it fit in? 
	auto childDisplacement = parent->childDisplacement();

	bool west = (point.x < (parent->origin.x + childDisplacement));
	bool north = (point.y < (parent->origin.y + childDisplacement));
	std::shared_ptr<LifeQuadTree::Node> subNode;
	if (north) subNode = (west) ? parent->northWest : parent->northEast;
	else subNode = (west) ? parent->southWest : parent->southEast;

	if (!subNode)
	{
		//create the subnode
		subNode = std::make_shared<LifeQuadTree::Node>();
		subNode->scale = parent->scale - 1;
		subNode->parent = parent;
		//Need to give appropriate values for x and y
		if (north)
		{
			subNode->origin.y = parent->origin.y;
			if (west) {
				subNode->origin.x = parent->origin.x;
				parent->northWest = subNode;
			}
				
			else {
				subNode->origin.x = parent->origin.x + parent->childDisplacement();
				parent->northEast = subNode;
			}
		}
		else
		{
			subNode->origin.y = parent->origin.y + parent->childDisplacement();
			if (west) {
				subNode->origin.x = parent->origin.x;
				parent->southWest = subNode;
			}
			else {
				subNode->origin.x = parent->origin.x + parent->childDisplacement();
				parent->southEast = subNode;
			}
		}
	}
	return subNode;
}

//Maybe I should return the node so that we can further process it if need be.
std::shared_ptr<LifeQuadTree::Node> LifeQuadTree::Tree::setLeaf(LifeQuadTree::Point point, bool alive)//should I add a decrement color value?
{
	//If correct parent could not be found, expand the tree.
	//Check to see if it is in bounding box of tree root. If not, expand the tree.
	while(!isInBoundingBox(point, rootNode->getBoundingBox())) expandTreeScaleTowardsPoint(point);

	auto currentNode = rootNode;
	while (currentNode->scale > 0) currentNode = descendTowardsPoint(currentNode, point);
	currentNode->setFlag(LifeQuadTree::IsAlive, alive);
	return currentNode;
}