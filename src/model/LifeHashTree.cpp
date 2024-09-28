#include "LifeHashTree.hpp"

void LifeHashTree::setCell(int x, int y, uint8_t state)
{
	if (!root_) root_ = std::make_shared<Node>();

	int shiftedx = x;
	int shiftedy = y;

	//Determine the tree depth required to contain x and y
	//It works because each the size of the tree doubles in each dimension every time you add a new root.
	//We must be sure that treeDepth_ never exceeds 31 so we don't shift out of our int!
	shiftedx >>= treeDepth_;
	shiftedy >>= treeDepth_;

	//Add root nodes if we need to.
	while (shiftedx != 0 || shiftedy != 0)
	{
		//They had an if (hashed) thing in here which I don't understand, 
		//So we'll leave it out for now.
		//They called a function pushroot_1();
		//I've renamed it to increaseTreeDepth();
		increaseTreeDepth_();
	}

	////Here is where they actually set the value.
	//root = gsetbit(root, x, y, newstate, depth);
	
}

bool LifeHashTree::increaseTreeDepth_()
{
	//Replace the children of root_ such that the old root children are centered in the new root children.
	std::shared_ptr<Node> newRootNW = std::make_shared<Node>();
	newRootNW->se = root_->nw;
	root_->nw = newRootNW;

	std::shared_ptr<Node> newRootNE = std::make_shared<Node>();
	newRootNE->sw = root_->ne;
	root_->ne = newRootNE;

	std::shared_ptr<Node> newRootSW = std::make_shared<Node>();
	newRootSW->ne = root_->sw;
	root_->sw = newRootSW;

	std::shared_ptr<Node> newRootSE = std::make_shared<Node>();
	newRootSE->nw = root_->se;
	root_->se = newRootSE;

	treeDepth_++;
}