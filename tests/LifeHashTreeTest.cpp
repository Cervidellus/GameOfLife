#include "../src/model/LifeHashTree.hpp"

#include <algorithm>
#include <format>
#include <iostream>


struct CellValue {
	int x;
	int y;
	int value;
};

void testInsertRetrieve(LifeHashTree &tree, std::vector<CellValue> values, bool expectSuccess = true)
{
	for (auto value : values)
	{
		tree.setCell(value.x, value.y, value.value);
	}

	for (auto value : values)
	{
		int retrieved = tree.getCell(value.x, value.y);//always returning 0;
		bool testPass = !expectSuccess;
		if (retrieved == value.value) testPass = expectSuccess ? true : false;
		testPass = expectSuccess ? (retrieved == value.value) : (retrieved != value.value);

		if (testPass) std::cout << std::format("Pass for CellValue:{} {} {}\n", value.x, value.y, value.value);
		else std::cout << std::format("********Fail******** for CellValue:{} {} {}\n", value.x, value.y, value.value);
	}
}

//These values start inserting in a clockwise pattern around zero so I can easily understand what the tree should look like.
//The last value requires a change in the size depth of the tree
const std::vector<CellValue> clockwiseInsertionValues{
	{0,0,1},
	{0,-1,2},
	{-1,-1,3},
	{-1,0,4},
	{0,1,5},
	{1,1,6},
	{1,0,7},
	{1,-1,8},
	{1,-2,9},
	{0,-2,10},
	{-1,-2,11},
	{-2,-2,12},
	{-2,-1,13},
	{-2,0,14},
	{-2,1,15},
	{-1,1,16},
	{0,4,17}//expect increase in tree depth to 3. Requires successful tree resizing for successful reads.
};

//Same as above, but write new values
const std::vector<CellValue> clockwiseNewValues{
{0,0,11},
{0,-1,12},
{-1,-1,13},
{-1,0,14},
{0,1,15},
{1,1,16},
{1,0,17},
{1,-1,18},
{1,-2,19},
{0,-2,110},
{-1,-2,111},
{-2,-2,112},
{-2,-1,113},
{-2,0,114},
{-2,1,115},
{-1,1,116},
{0,4,117}//expect increase in tree depth to 3. Requires successful tree resizing for successful reads.
};

const std::vector<CellValue> largeCellValues{
	{1, 1, 5},
	{0, 1, 10},
	{1, 0, 120},
	{ -1, 1, 255 },
	{ 55, 0, 100 },
	{ -500, 15, 255 },
	//Special case of insertion of an initial zero value. Will not allocate, but should return 0 correctly.
	{ 200, -1000, 0 },
	{ -10000, -12, 55 },
	{ 0, 0, 99 },
	{ 102, 11, 22 },
	{7348, 1, 25},
	{-53454, 43236, 2}
};

std::vector<CellValue> outOfRangeCellValues
{
	{1,1,256},
	{2,2,2000},
	{3,3,-1},
	{4,4,-5000}
};

//Make sure behavior works setting things back to zero
std::vector<CellValue> zeroValues
{
	{0,0,0},
	{0,-1,0},
	{-1,-1,0},
	{-1,0,0},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,-1,0},
	{1,-2,0},
	{0,-2,0},
	{-1,-2,0},
	{-2,-2,0},
	{-2,-1,0},
	{-2,0,0},
	{-2,1,0},
	{-1,1,0},
	{0,4,0}//expect increase in tree depth to 3. Requires successful tree resizing for successful reads.
};

int main(int argc, char** argv) {
	LifeHashTree tree;
	std::cout << "\n****Testing Clockwise Insertion of values, with the last value resizing the tree.\n";
	testInsertRetrieve(tree, clockwiseInsertionValues);
	std::cout << "\n****Testing setting new values to those previously set.\n";
	testInsertRetrieve(tree, clockwiseNewValues);
	std::cout << "\n****Testing larger values.\n";
	testInsertRetrieve(tree, largeCellValues);
	std::cout << "\n****Testing out of range values. Pass means the values were not retrieved correctly.\n";
	testInsertRetrieve(tree, outOfRangeCellValues, false);
	std::cout << "\n****Testing resetting values to zero.\n";
	testInsertRetrieve(tree, zeroValues);
	return 0;
}
