#include "LifeHashTree.hpp"

void LifeHashTree::setCell(int x, int y, uint8_t state)
{
    //std::variant ALWAYS holds something.. in this case of default of a Node. 
    // It will be initialized with an empty node. 
	if (!root_) root_ = std::make_shared<NodeOrLeaf>(Node());

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
        shiftedx >>= 1;
        shiftedy >>= 1;
	}

	////Here is where they actually set the value.
	//root = gsetbit(root, x, y, newstate, depth);
	
}

bool LifeHashTree::increaseTreeDepth_()
{
	//Replace the children of root_ such that the old root children are centered in the new root children.
    //This modifies the root in place and swaps in new children at 1 higher depth.
    Node* root = std::get_if<Node>(root_.get());
    if (!root) return false;

    //Construct the Node that will be the NW child of the new root.
    auto newChildNW = std::make_shared<NodeOrLeaf>(Node{});
    //Swap in what was the nw child of the old root into the se child position of the new child. 
    std::get<Node>(*newChildNW).se.swap(root->nw);
    //Now that the nw child of the new root it complete, swap it in.
    root->nw.swap(newChildNW);

    auto newChildNE = std::make_shared<NodeOrLeaf>(Node{});
    std::get<Node>(*newChildNE).sw.swap(root->ne);
    root->ne.swap(newChildNE);

    auto newChildSW = std::make_shared<NodeOrLeaf>(Node{});
    std::get<Node>(*newChildSW).ne.swap(root->sw);
    root->sw.swap(newChildSW);

    auto newChildSE = std::make_shared<NodeOrLeaf>(Node{});
    std::get<Node>(*newChildSE).nw.swap(root->se);
    root->se.swap(newChildSE);

	treeDepth_++;
    return true;
}


//ghashbase::gsetbit
std::shared_ptr<LifeHashTree::NodeOrLeaf> LifeHashTree::recursiveSetCellState_(
    std::shared_ptr<NodeOrLeaf> nodeOrLeaf, 
    int x, 
    int y, 
    int newState, 
    int nodeDepth)
{
    //nodeOrLeaf is a Node. This will include empty NodeOrLeaf.
    //auto node = std::get_if<Node>(nodeOrLeaf.get());
    if (nodeDepth > 0/* && auto node = std::get_if<Node>(nodeOrLeaf.get())*/ /*&& nodeDepth > 0*/)
    {
        auto node = std::get_if<Node>(nodeOrLeaf.get());
        
        //avoiding division. We can bit shift because of the quad tree scaling by 2 in each dimension.
        int width = 1 << nodeDepth;
        int halfWidth = 1 << (nodeDepth - 1);
        nodeDepth--;


        std::shared_ptr<NodeOrLeaf> childPointer;

        // Determine which quadrant to traverse
        //if (nodeDepth + 1 == treeDepth_ || depth < 31) {
        if (x < 0) {
            if (y < 0)
                childPointer = node->sw;
            else
                childPointer = node->nw;
        }
        else {
            if (y < 0)
                childPointer = node->se;
            else
                childPointer = node->ne;
        }


        // Create a new node if the child pointer is null
        if (!childPointer) {
            if (nodeDepth == 0) childPointer = std::make_shared<NodeOrLeaf>(Leaf{});
            else childPointer = std::make_shared<NodeOrLeaf>(Node{});
        }

        // Recursively set the bit in the child node
        //Bitwise operators make sure that they are within range.. I should technically be able to get rid of that?
        //Subtracting the halfwidth realigns the coordinate space to the selected child node.
        std::shared_ptr<LifeHashTree::NodeOrLeaf> updatedChild = recursiveSetCellState_(childPointer, (x & (width - 1)) - halfWidth,
            (y & (width - 1)) - halfWidth, newState, nodeDepth);

        //This is for caching. 
        // Update the parent node with the new child node
        //if (hashed) {
        //    ghnode* nw = (childPointer == &(node->nw) ? updatedChild : node->nw);
        //    ghnode* sw = (childPointer == &(node->sw) ? updatedChild : node->sw);
        //    ghnode* ne = (childPointer == &(node->ne) ? updatedChild : node->ne);
        //    ghnode* se = (childPointer == &(node->se) ? updatedChild : node->se);
        //    node = save(find_ghnode(nw, ne, sw, se));
        //}
        //else {
        //    childPointer = updatedChild;
        //}
        //return node;

    }
    
    //NodeOrLeaf is a Leaf, the base case that stops recursion.
    else
    {
        //Set the state of the correct leaf
        auto leaf = std::get_if<Leaf>(nodeOrLeaf.get());
        if (leaf)
        {
            //set the correct quadrant value
            //For now with no caching
            if (x < 0)
            {
                if (y < 0) leaf->sw = newState;
                else leaf->nw = newState;
            }
            else
            {
                if (y < 0) leaf->se = newState;
                else leaf->nw = newState;
            }
        }
    }

    return nodeOrLeaf;
}

//
//
//
//
//
//
//
//
////CHatgpt version wiht explicit names and explanation:
//ghnode* ghashbase::gsetbit(ghnode* node, int x, int y, int newState, int depth) {
//    if (depth == 0) {
//        // Cast the node to a leaf node
//        ghleaf* leaf = (ghleaf*)node;
//        if (hashed) {
//            // Save the current states of the quadrants
//            state nwState = leaf->nw;
//            state swState = leaf->sw;
//            state neState = leaf->ne;
//            state seState = leaf->se;
//
//            // Set the new state based on the coordinates
//            if (x < 0) {
//                if (y < 0)
//                    swState = (state)newState;
//                else
//                    nwState = (state)newState;
//            }
//            else {
//                if (y < 0)
//                    seState = (state)newState;
//                else
//                    neState = (state)newState;
//            }
//
//            // Save and return the updated node
//            return save((ghnode*)find_ghleaf(nwState, neState, swState, seState));
//        }
//
//        // Set the new state directly in the leaf node
//        if (x < 0) {
//            if (y < 0)
//                leaf->sw = (state)newState;
//            else
//                leaf->nw = (state)newState;
//        }
//        else {
//            if (y < 0)
//                leaf->se = (state)newState;
//            else
//                leaf->ne = (state)newState;
//        }
//        return (ghnode*)leaf;
//    }
//    else {
//        unsigned int width = 0, halfWidth = 0;
//        if (depth > 31) {
//            if (depth == 32)
//                halfWidth = 0x80000000;
//            width = 0;
//        }
//        else {
//            width = 1 << depth;
//            halfWidth = 1 << (depth - 1);
//        }
//        depth--;
//
//        ghnode** childPointer;
//
//        // Determine which quadrant to traverse
//        if (depth + 1 == this->depth || depth < 31) {
//            if (x < 0) {
//                if (y < 0)
//                    childPointer = &(node->sw);
//                else
//                    childPointer = &(node->nw);
//            }
//            else {
//                if (y < 0)
//                    childPointer = &(node->se);
//                else
//                    childPointer = &(node->ne);
//            }
//        }
//        else {
//            if (x >= 0) {
//                if (y >= 0)
//                    childPointer = &(node->sw);
//                else
//                    childPointer = &(node->nw);
//            }
//            else {
//                if (y >= 0)
//                    childPointer = &(node->se);
//                else
//                    childPointer = &(node->ne);
//            }
//        }
//
//        // Create a new node if the child pointer is null
//        if (*childPointer == 0) {
//            if (depth == 0)
//                *childPointer = (ghnode*)newclearedghleaf();
//            else
//                *childPointer = newclearedghnode();
//        }
//
//        // Recursively set the bit in the child node
//        ghnode* updatedChild = gsetbit(*childPointer, (x & (width - 1)) - halfWidth,
//            (y & (width - 1)) - halfWidth, newState, depth);
//
//        // Update the parent node with the new child node
//        if (hashed) {
//            ghnode* nw = (childPointer == &(node->nw) ? updatedChild : node->nw);
//            ghnode* sw = (childPointer == &(node->sw) ? updatedChild : node->sw);
//            ghnode* ne = (childPointer == &(node->ne) ? updatedChild : node->ne);
//            ghnode* se = (childPointer == &(node->se) ? updatedChild : node->se);
//            node = save(find_ghnode(nw, ne, sw, se));
//        }
//        else {
//            *childPointer = updatedChild;
//        }
//        return node;
//    }
//}


//Explanation:
//Base Case(Leaf Node) :
//    If depth is 0, the function treats the node as a leaf(ghleaf).
//    It sets the state of the appropriate quadrant(nw, sw, ne, se) based on the coordinates(x, y).
//    If hashed is true, it saves the new state and returns the updated node.
//    Recursive Case :
//If depth is greater than 0, it calculates the width and half - width of the current level.
//It determines which child node to traverse based on the coordinates(x, y).
//If the child node is null, it creates a new cleared node.
//It recursively calls gsetbit on the child node with updated coordinates and depth.
//After the recursive call, it updates the parent node with the new child node.