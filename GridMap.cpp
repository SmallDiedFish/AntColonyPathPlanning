/*
 * Copyright (c) 2024 Ch Oy @ NWPU
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "GridMap.h"
#include <iostream>

GridMap::GridMap(int width, int height) : width(width), height(height), grid(height, std::vector<bool>(width, false)) {}

void GridMap::markObstacle(int x, int y) {
    grid[y][x] = true;
}

void GridMap::markObstacles(const std::vector<std::pair<int, int>>& obstacles) {
    for (const auto& obstacle : obstacles) {
        markObstacle(obstacle.first, obstacle.second);
    }
}

bool GridMap::isObstacle(int x, int y) const {
    return grid[y][x];
}

void GridMap::printMap() const {
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x] ? "X" : ".") << " ";
        }
        std::cout << std::endl;
    }
}

int GridMap::getMaxSteps() const {
    return (int)100;
//    return width * height;
}
