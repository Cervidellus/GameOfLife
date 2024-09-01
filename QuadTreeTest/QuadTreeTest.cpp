
#include <iostream>
#include "../src/model/LifeQuadTree.hpp"
#include "../src/model/LifeQuadTreeModel.hpp"

struct TestResult
{
    bool success = true;
    std::string resultString = "";
};

TestResult testSetLeaf(LifeQuadTree::Tree& tree, const LifeQuadTree::Point newPoint, const bool alive)
{
    TestResult result;
    auto testNode = tree.setLeaf(newPoint, alive);
    if (!LifeQuadTree::isInBoundingBox(newPoint, tree.rootNode->getBoundingBox()))
    {
        result.success = false;
        result.resultString += "New Point is Not In Tree Root Bounding Box.\n";
    }

    if (testNode->origin != newPoint)
    {
        result.success = false;
        result.resultString += "Origin for set node does not match test point.\n";
    }

    if (!testNode->getFlag(LifeQuadTree::NodeFlagNames::IsAlive) == alive)
    {
        result.success = false;
        result.resultString += "IsAlive flag incorrect for test point.\n";
    }

    result.resultString += (result.success) ? "Test passed.\n" : "Test failed.\n";
    return result;
}

int main()
{
    LifeQuadTree::Tree tree;

    auto result = testSetLeaf(tree, LifeQuadTree::Point{ 0, 0 }, true);
    std::cout << "Test result for Point{0,0}:\n";
    std::cout <<  result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ 3, 3 }, true);
    std::cout << "Test result for Point{3, 3}:\n";
    std::cout << result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ -5, -44 }, true);
    std::cout << "Test result for Point{-5, -44}:\n";
    std::cout << result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ 16, -22 }, true);
    std::cout << "Test result for Point{3, 3}:\n";
    std::cout << result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ 3, 3 }, false);
    std::cout << "Test result for Point{0,0}:\n";
    std::cout << result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ 3, 22 }, false);
    std::cout << "Test result for Point{0,0}:\n";
    std::cout << result.resultString;

    LifeQuadTreeModel model;
    //model.initialize();

    std::cout << "Test complete.\n";
}
