#ifndef LIFEHASH_TREE_HPP
#define LIFEHASH_TREE_HPP

#include <array>
#include <memory>
#include <unordered_map>
#include <variant>

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


//I don't really understand 100% how they are using a hash table, but I need to make one. 

//I need a data structure to store it in,a hash function, and a collision handling scheme.
// 
// FOr the data structure, I think I will just use std::unordered_map at first..
// Not the most optimal, but easy to implement. 
// 
// For the hash function:
//I'm storing stuff as shared_ptr.. so I theoretically could hash the shared_ptr address with std::hash
//Or should I copy the hash function that they have done?
// FNV-1a seems like a performant hash algorithm?

//For probing collisions, I could try double hashing? So I need two hashing functions. 
//Or robin hood probing? https://www.youtube.com/watch?v=IMnbytvHCjM

class LifeHashTree {
public:
	LifeHashTree() {};
	~LifeHashTree() {};

	struct InternalNode;
	struct TerminalNode;
	typedef std::variant<InternalNode, TerminalNode> Node;

	//ghashbase::setcell
	void setCell(int x, int y, uint8_t state);

	//ghbase::getcell
	int getCell(int x, int y);

	//I am using shared_ptr as I inend to implement garbage collection.
	//I might decide to change to unique_ptr for slightly less overhead.
	struct InternalNode {
		std::shared_ptr<Node> nw;
		std::shared_ptr<Node> ne;
		std::shared_ptr<Node> sw;
		std::shared_ptr<Node> se;
		//leaving these commented out until I understand them. They are in the original algo.
		//std::shared_ptr<Node> next;//Maybe this just to iterate over the nodes?
		//std::shared_ptr<Node> res;//This is the cache?
	};

	//terminal nodes contain 4 leaves
	struct TerminalNode {
		uint8_t nw = 0;
		uint8_t ne = 0;
		uint8_t sw = 0;
		uint8_t se = 0;
		//THey had a leafpop and a bool isleaf.
		//I don't understand leafpop, and we don't need isleaf if we are a leaf!
		//They also have a next pointer that I don't yet understand.
	};

private:
	//Create a new root node, centered on the old root.
	bool increaseTreeDepth_();


	//ghashbase::gsetbit
	void recursiveSetCellState_(
		std::shared_ptr<Node> nodeOrLeaf, 
		int x, 
		int y, 
		int newstate, 
		int nodeDepth
	);

	int recursiveGetCellState_(
		std::shared_ptr<Node> nodeOrLeaf,
		int x,
		int y,
		int nodeDepth
	);

	std::shared_ptr<Node> root_ = std::make_shared<Node>(InternalNode());
	std::unordered_map<int, Node> hashTable_;
	// The minimum root node depth is 2. Depth 1 is a terminal node. depth 0 is an individual cell (or leaf). 
	int treeDepth_ = 2;
};

#endif // LIFEHASH_TREE_HPP
