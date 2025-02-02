#include "LifeHashTree.hpp"
#include <iostream>
#include <cmath>
#include <format>

void LifeHashTree::setCell(int x, int y, uint8_t state)
{
	//Determine the tree depth required to contain x and y
	//Binary operations work because the size of the tree doubles in each dimension every time you add a new root.
	//We must be sure that treeDepth_ never exceeds 31 so we don't shift out of our int!
    int shiftedx = std::abs(x);
    int shiftedy = std::abs(y);
	shiftedx >>= (treeDepth_ );
	shiftedy >>= (treeDepth_ );

	//Add root nodes if we need to.
	while (shiftedx != 0 || shiftedy != 0)
	{
		//Original algo had an if (hashed) thing in here which I don't understand, 
		//So we'll leave it out for now.
		increaseTreeDepth_();
        shiftedx >>= 1;
        shiftedy >>= 1;
	}

    //Navigate the tree to set the bit.
    recursiveSetCellState_(root_, x, y, state, treeDepth_);
}

int LifeHashTree::getCell(int x, int y)
{
    int shiftedx = std::abs(x);
    int shiftedy = std::abs(y);
    shiftedx >>= treeDepth_;
    shiftedy >>= treeDepth_;

    //If the point is outside the bounds of the quadtree, return 0;
    if (shiftedx != 0 || shiftedy != 0) return 0;
    return recursiveGetCellState_(root_, x, y, treeDepth_);
}

bool LifeHashTree::increaseTreeDepth_()
{
	//Replace the children of root_ such that the old root children are centered in the new root children.
    //This modifies the root in place and swaps in new children at 1 higher depth.
    InternalNode* root = std::get_if<InternalNode>(root_.get());
    if (!root) return false;

    //I need to modify to better handle the zero value case, so I don't need to navigate all the way down.
    //Construct the Node that will be the NW child of the new root.
    if (root->nw)
    {
        auto newChildNW = std::make_shared<Node>(InternalNode{});
        //Swap in what was the nw child of the old root into the se child position of the new child. 
        std::get<InternalNode>(*newChildNW).se.swap(root->nw);
        //Now that the nw child of the new root it complete, swap it in.
        root->nw.swap(newChildNW);
    }

    if (root->nw)
    {
        auto newChildNE = std::make_shared<Node>(InternalNode{});
        std::get<InternalNode>(*newChildNE).sw.swap(root->ne);
        root->ne.swap(newChildNE);
    }

    if (root->sw)
    {
        auto newChildSW = std::make_shared<Node>(InternalNode{});
        std::get<InternalNode>(*newChildSW).ne.swap(root->sw);
        root->sw.swap(newChildSW);
    }

    if (root->se)
    {
        auto newChildSE = std::make_shared<Node>(InternalNode{});
        std::get<InternalNode>(*newChildSE).nw.swap(root->se);
        root->se.swap(newChildSE);
    }

	treeDepth_++;
    return true;
}

void LifeHashTree::recursiveSetCellState_(
    std::shared_ptr<Node> node, 
    int x, 
    int y, 
    int newState, 
    int nodeDepth)
{
    if (auto internalNode = std::get_if<InternalNode>(node.get()))
    {
        //avoiding division. We can bit shift because of the quad tree scaling by 2 in each dimension.
        int width = 1 << nodeDepth;
        int halfWidth = 1 << (nodeDepth - 1);

        //Which quadrant should it be in?
        auto& childPointer = (x < 0) ? ((y < 0) ? internalNode->sw : internalNode->nw) : ((y < 0) ? internalNode->se : internalNode->ne);
        if (!childPointer) {
            //No need to allocate if newState is zero.
            if (newState == 0) return;
            //Allocate child to either internal or terminal node
            childPointer = (nodeDepth == 2) ? std::make_shared<Node>(TerminalNode{}) : std::make_shared<Node>(InternalNode{});
        }

        nodeDepth--;
        recursiveSetCellState_(
            childPointer,
            (x & (width -1)) - halfWidth,//this tells us which quadrant to look in at the next level down
            (y & (width - 1)) - halfWidth,
            newState, 
            nodeDepth);
    }
    
    //Bae Case where the node is a terminal node.
    else
    {
        //Determine x and y that will tell us which leaf in the terminal node to set
        int width = 1 << nodeDepth;
        int halfWidth = 1 << (nodeDepth - 1);
        x = (x & (width - 1)) - halfWidth;
        y = (y & (width - 1)) - halfWidth;
        //Set the state of the correct leaf
        auto terminalNode = std::get_if<TerminalNode>(node.get());
        //TODO: I might want to add garbage collection to occassionally clear terminal nodes that become empty.
        //Doing it aggressively would slow things down as cells will often flicker between zero and non zero states.
        if (x < 0)
        {
            if (y < 0) terminalNode->sw = newState;
            else terminalNode->nw = newState;
        }
        else
        {
            if (y < 0) terminalNode->se = newState;
            else terminalNode->ne = newState;
        }
    }
}

//ghashbase::getcell
int LifeHashTree::recursiveGetCellState_(
    std::shared_ptr<Node> nodeOrLeaf,
    int x,
    int y,
    int nodeDepth)
{
    int width = 1 << nodeDepth;
    int halfWidth = 1 << (nodeDepth - 1);
    //check if leaf node. Base case to stop recursion.
    if (auto terminalNode = std::get_if<TerminalNode>(nodeOrLeaf.get()))
    {
        //navigate to next level down
        int width = 1 << nodeDepth;
        int halfWidth = 1 << (nodeDepth - 1);
        x = (x & (width - 1)) - halfWidth;
        y = (y & (width - 1)) - halfWidth;

        return (x < 0) ? ((y < 0) ? terminalNode->sw : terminalNode->nw) : ((y < 0) ? terminalNode->se : terminalNode->ne);
    }
    auto node = std::get_if<InternalNode>(nodeOrLeaf.get());
    auto& childPointer = (x < 0) ? ((y < 0) ? node->sw : node->nw) : ((y < 0) ? node->se : node->ne);

    //Stop recursion if empty
    if (!childPointer) return 0;

    nodeDepth--;
    return recursiveGetCellState_(
        childPointer,
        (x & (width - 1)) - halfWidth,
        (y & (width - 1)) - halfWidth,
        nodeDepth);
}