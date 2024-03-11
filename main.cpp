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
#include "AntColony.h"
#include <iostream>

int main() {
    // initialize a 25x25 grid map，marking some obstacles
    GridMap map(25, 25);
    std::vector<std::pair<int, int>> obstacles = {// set obstacles
    {4, 21}, {4, 22},
    {5, 18},
    {6, 10}, {6, 11},
    {7, 5}, {7, 6},
    {8, 5}, {8, 6},
    {10, 14},
    {11, 14},
    {12, 9}, {12, 14}, {12, 19}, {12, 20},
    {13, 9}, {13, 14}, {13, 19}, {13, 20}, {13, 21},
    {14, 14}, {14, 20},
    {15, 2}, {15, 3}, {15, 14},
    {16, 2}, {16, 3},
    {17, 10},
    {18, 10},
    {21, 0}, {21, 1}, {21, 2}, {21, 3}, {21, 4}, {21, 5}, {21, 6}, {21, 7}, {21, 8},
    {22, 20}
    };
    map.markObstacles(obstacles);

    int start_x, start_y, end_x, end_y;

    std::cout << "input start coordinates(divide via space)";
    std::cin >> start_x >> start_y ;
    std::cout << "input ending coordinates(divide via space)";
    std::cin >> end_x >> end_y;

    std::pair<int, int> start(start_x, start_y);
    std::pair<int, int> end(end_x, end_y); 
    //std::pair<int, int> start(0, 24); 
    //std::pair<int, int> end(24, 0); 
  
    AntColony antColony(map, 100, 10, start, end); // initialize Ant Colony algorithm where 100 is ants counts and 10 is max iteration
    antColony.run(); // run Ant Colony algorithm

    antColony.printBestPath(); // print best path

    return 0;
}
