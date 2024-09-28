#ifndef LIFEHASH_TREE_HPP
#define LIFEHASH_TREE_HPP

#include <array>
#include <memory>

//This is intended to be based on the HashLife implementation as a part of the Golly project for cellular automata.
//https://sourceforge.net/projects/golly/

//My implementation will allow for 8 bit color values for each cell, instead of a single bit.
//THis should slow it down, but allow for nicer visualizations.

//Life Hashlife, I store cells in 16*16 blocks, and use a quadtree to store the blocks.
//Results for blocks are stored in a hash table, so that they can be reused.


//What I need:
//1. A struct to store a node that offers pointers to 4 children. 
// Golly is also storing a next and a res, so I have to figure out what that is for.
//I'm going to use shared_ptr instead of raw pointers.

//2. A struct to store leaf nodes. 
//It offers 4 states for nw, ne, sw, and se. 
//again, golly offers a next and isghnode, but I don't think isghnode is used? 
//They are also offering a bigint leafpop, which I don't know what is for.
//THey say it is for "how many set bits"

//3. A class to store the tree and a set of methods for dealing with it. 
// This is kind of like my CPUModel class. Their class is more complicated than I would like.
//I'll try and remove as much as I can, and/or move to other classes/namespaces...

//I will additionally have some methods for generating random initial data. 


//ghashbase::step() is the main function called.

//I think hte first steps are to get the ability to assemble a tree working. 

struct Node {
	std::shared_ptr<Node> nw;
	std::shared_ptr<Node> ne;
	std::shared_ptr<Node> sw;
	std::shared_ptr<Node> se;
	//leaving these commented out until I understand them.
	//std::shared_ptr<Node> next;//Maybe this just to iterate over the nodes?
	//std::shared_ptr<Node> res;//This is the cache?
};

class LifeHashTree {
public:
	LifeHashTree() {};
	~LifeHashTree() {};

	void setCell(int x, int y, uint8_t state);

private:
	//Create a new root node, centered on the old root.
	bool increaseTreeDepth_();

	std::shared_ptr<Node> root_;
	int treeDepth_ = 0; 
};

#endif // LIFEHASH_TREE_HPP