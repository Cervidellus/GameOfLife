#ifndef VECTORGRID_TEST
#define VECTORGRID_TEST
#include "../src/model/VectorGrid.hpp"

#include <iostream>
#include <stdexcept>

namespace VectorGridTest
{
    static void testResize() {
        VectorGrid grid;
        grid.resize(3, 3);
        if (grid.rows() == 3 && grid.columns() == 3) {
            std::cout << "Resize test passed.\n";
        }
        else {
            std::cout << "Resize test failed.\n";
        }
    }

    static void testMooreNeighborhood() {
        VectorGrid grid;
        grid.resize(3, 3);
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                grid(i, j) = i * 3 + j;
            }
        }

        std::vector<uint8_t> expected_neighbors = { 0, 1, 2, 3, 5, 6, 7, 8 };
        std::vector<uint8_t> neighbors;

        std::size_t x = 1, y = 1;
        for (std::size_t i = x - 1; i <= x + 1; ++i) {
            for (std::size_t j = y - 1; j <= y + 1; ++j) {
                if (i == x && j == y) continue;
                neighbors.push_back(grid(i, j));
            }
        }

        if (neighbors == expected_neighbors) {
            std::cout << "Moore neighborhood test passed.\n";
        }
        else {
            std::cout << "Moore neighborhood test failed.\n";
        }
    }

    static void testZero() {
        VectorGrid grid;
        grid.resize(3, 3);
        grid.zero();
        bool passed = true;
        for (std::size_t i = 0; i < grid.rows(); ++i) {
            for (std::size_t j = 0; j < grid.columns(); ++j) {
                if (grid(i, j) != 0) {
                    passed = false;
                    break;
                }
            }
        }
        if (passed) {
            std::cout << "Zero test passed.\n";
        }
        else {
            std::cout << "Zero test failed.\n";
        }
    }

    static void testOperatorAccess() {
        VectorGrid grid;
        grid.resize(3, 3);
        grid(2, 2) = 42;
        if (grid(2, 2) == 42) {
            std::cout << "Operator access test passed.\n";
        }
        else {
            std::cout << "Operator access test failed.\n";
        }
    }

    static void run()
    {
        std::cout << "Running Tests for VectorGrid.\n";
        testResize();
        testMooreNeighborhood();
        testZero();
        testOperatorAccess();
    }

}


#endif //VECTORGRID_TEST