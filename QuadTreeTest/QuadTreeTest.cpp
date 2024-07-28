// QuadTreeTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../src/model/LifeQuadTree.hpp"

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

    //LifeQuadTree::Node node;
    //node.scale = 1;
    //auto scale1 = node.childDisplacement();//expect 1
    //node.scale = 2;
    //auto scale2 = node.childDisplacement();//expect 2
    //node.scale = 3;
    //auto scale3 = node.childDisplacement();//expect 4
    //node.scale = 4;
    //auto scale4 = node.childDisplacement();//expect 8

    auto result = testSetLeaf(tree, LifeQuadTree::Point{ 0, 0 }, true);
    std::cout << "Test result for Point{0,0}:\n";
    std::cout <<  result.resultString;

    result = testSetLeaf(tree, LifeQuadTree::Point{ 3, 3 }, true);//testnode origin is 2,2!
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

    //auto startOrigin = testnode->origin;
    //Origin should stay the same. 
    //auto testNode = tree.setLeaf(LifeQuadTree::Point(3, 3), true);
    //if (tree.rootNode->origin.x != startOrigin.x && tree.rootNode->origin.y != startOrigin.y)
    //{
    //    std::cout << "Wrong tree root after adding Point(3,3)." << "\n";//It got set to 3,3!
    //}

    //testnode = tree.setLeaf(LifeQuadTree::Point{ 12,33 }, true);
    ////This should also keep the origin the same. Never returns!
    //if (tree.rootNode->origin.x != startOrigin.x && tree.rootNode->origin.y != startOrigin.y)
    //{
    //    std::cout << "Wrong tree root after adding Point(12,33)." << "\n";
    //}
    ////This should move the origin to the northwest to enclose (-5,-44)
    //testnode = tree.setLeaf(LifeQuadTree::Point{ -5,-44 }, true);

    //testnode = tree.setLeaf(LifeQuadTree::Point{ 0,3 }, false);
    
}
