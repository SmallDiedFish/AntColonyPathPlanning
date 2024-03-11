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
#include "AntColony.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

AntColony::AntColony(GridMap& map, int antCount, int maxIterations, std::pair<int, int> start, std::pair<int, int> end)
    : map(map), antCount(antCount), maxIterations(maxIterations), start(start), end(end) {
    initializePheromones();
}

void AntColony::initializePheromones() {
    pheromones = std::vector<std::vector<double>>(map.getWidth(), std::vector<double>(map.getHeight(), 1.0));
}

void AntColony::initializeAnts() {
    ants.clear();
    for (int i = 0; i < antCount; ++i) {
        ants.emplace_back(start);
    }
}

void AntColony::evaluateAndUpdateBestPath(Ant& ant) {
    int currentPathLength = calculatePathLength(ant.path);
    ant.pathLength = currentPathLength;
    if (currentPathLength < bestPathLength) {
        bestPathLength = currentPathLength;
        bestPath = ant.path;
    }
}

void AntColony::moveAnts() {
    for (auto& ant : ants) {
        if ((ant.currentPosition == end) && (!ant.arrived)) {
            ant.arrived = true;
            // 信息素沉积
            for (auto& ant : ants) {
                for (auto& node : ant.path) {
                    // 可叠加信息素
                    pheromones[node.first][node.second] += Q / ant.pathLength;

                    ////不可叠加信息素
                    //double pheromoneCandidate = Q / ant.pathLength;
                    ///*double pheromoneCandidate = Q / pow(ant.pathLength, 2);*/
                    //if (pheromoneCandidate > pheromones[node.first][node.second])
                    //    pheromones[node.first][node.second] = pheromoneCandidate;
                }
            }
            evaluateAndUpdateBestPath(ant);
            
            //到达后返回起点再来一次
            //ant.path.clear(); // 清空当前路径
            //ant.path.emplace_back(start);
            //ant.currentPosition = start; // 返回起点
        }
        if (!ant.arrived) {
            auto nextNodes = getFeasibleNextNodes(ant);
            if (!nextNodes.empty()) {
                int nextNodeIndex = chooseNextNode(ant);
                ant.currentPosition = nextNodes[nextNodeIndex];
                ant.path.push_back(ant.currentPosition);
                ant.pathLength = calculatePathLength(ant.path);
            }
        }
    }
    //printAntsDistribution();
}

void AntColony::printAntsDistribution() const {
    // 创建栅格来存储蚂蚁的数量
    std::vector<std::vector<int>> antsDistribution(map.getHeight(), std::vector<int>(map.getWidth(), 0));

    // 计算每个方格中的蚂蚁数量
    for (const auto& ant : ants) {
        int x = ant.currentPosition.first;
        int y = ant.currentPosition.second;
        antsDistribution[y][x]++; 
    }

    //画图
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            if (map.isObstacle(x, y)) {
                std::cout << "X"; // 使用X表示障碍物
            }
            else if (antsDistribution[y][x] > 0) {
                std::cout << antsDistribution[y][x]; // 打印蚂蚁的数量
            }
            else {
                std::cout << " "; // 使用点表示空白区域
            }
        }
        std::cout << std::endl;
    }
}

int AntColony::chooseNextNode(const Ant& ant) {
    auto nextNodes = getFeasibleNextNodes(ant);
    std::vector<double> probabilities(nextNodes.size(), 0.0);
    double probabilitySum = 0.0;

    for (size_t i = 0; i < nextNodes.size(); ++i) {
        auto node = nextNodes[i];
        double pheromone = pow(pheromones[node.first][node.second], alpha);
        // 以距离的倒数作为启发式信息
        double heuristic = 1.0 / sqrt(pow(node.first - end.first, 2) + pow(node.second - end.second, 2)); // 直线距离
        //int heuristic = abs(node.first - end.first) + abs(node.second - end.second); // 栅格距离 可能算的快一点
        double probability = pheromone * pow(heuristic, beta);
        probabilities[i] = probability;
        probabilitySum += probability;
    }

    // 轮盘赌选择下一个节点
    double randomValue = (double)rand() / RAND_MAX * probabilitySum;
    double cumulativeProbability = 0.0;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        cumulativeProbability += probabilities[i];
        if (randomValue <= cumulativeProbability) {
            return i;
        }
    }
    return 0; // 默认返回第一个节点作为后备
}

std::vector<std::pair<int, int>> AntColony::getFeasibleNextNodes(const Ant& ant) const {
    std::pair<int, int> position = ant.currentPosition;
    std::vector<std::pair<int, int>> nextNodes;
    const std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }; // 左 右 上 下

    for (auto& dir : directions) {
        std::pair<int, int> nextPosition = { position.first + dir.first, position.second + dir.second };
        if (nextPosition.first >= 0 && nextPosition.first < map.getWidth() &&
            nextPosition.second >= 0 && nextPosition.second < map.getHeight() &&
            !map.isObstacle(nextPosition.first, nextPosition.second)) {
            if (ant.path.size() == 1) {
                nextNodes.push_back(nextPosition);
            }else {
                if (nextPosition != ant.path[ant.path.size() - 2]) //防止走回头路
                    nextNodes.push_back(nextPosition);
            }
        }
    }
    return nextNodes;
}

void AntColony::updatePheromones() {
    // 信息素蒸发
    for (auto& row : pheromones) {
        for (auto& pheromone : row) {
            pheromone *= (1.0 - evaporationRate);
        }
    }

    //// 信息素沉积
    //for (auto& ant : ants) {
    //    if (ant.arrived) {
    //        for (auto& node : ant.path) {
    //            //pheromones[node.first][node.second] += Q / pow(ant.pathLength, 2);// 可叠加信息素
    //            
    //            //不可叠加信息素
    //            double pheromoneCandidate = Q / ant.pathLength;
    //            /*double pheromoneCandidate = Q / pow(ant.pathLength, 2);*/
    //            if (pheromoneCandidate > pheromones[node.first][node.second])
    //                pheromones[node.first][node.second] = pheromoneCandidate;
    //        }
    //    }

    //}

    //// 信息素沉积
    //for (auto& ant : ants) {
    //    for (auto& node : ant.path) {
    //        pheromones[node.first][node.second] += Q / pow(ant.pathLength, 2);
    //    }
    //}
}

void AntColony::run() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // 设置光标不可见
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    int maxSteps = getMaxSteps();
    std::ofstream BPLfile("BestPathLength.csv");
    std::ofstream APLfile("AveragePathLength.csv");
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        progressBar(iteration, maxIterations, "Iteration", 50, 0);
        initializeAnts();
        for (int step = 0; step < maxSteps; ++step) {
            progressBar(step, maxSteps, "Step     ", 50, 1);;
            moveAnts();
        }
        /*for (auto& ant : ants) {
            evaluateAndUpdateBestPath(ant);
        }*/
        updatePheromones();
        std::cout << std::endl << "Iteration:" << iteration << std::endl;
        printPheromones();
        savePheromoneMatrixToCSV(pheromones, iteration);
        BPLfile << bestPathLength << "\n";
        int sumArrivedPathLength= 0;
        int antArrivedCount = 0;
        for (auto& ant : ants) {
            if (ant.arrived) {
                sumArrivedPathLength += ant.pathLength;
                antArrivedCount++;
            }  
        }
        double averagePathLength = sumArrivedPathLength / (double)antArrivedCount;
        APLfile << averagePathLength << "\n";
    }
    //printBestPath();
}

int AntColony::getMaxSteps() const { 
    int deltaX = abs(start.first - end.first);
    int deltaY = abs(start.second - end.second);
    return (deltaX + deltaY) * 6;
    //return (deltaX + 2) * (deltaY + 2);
}

int AntColony::calculatePathLength(const std::vector<std::pair<int, int>>& path) const {
    double length = 0;
    length = path.size() - 1;
    return length;
}

void AntColony::printBestPath() const {
    if (bestPath.empty()) {
        std::cout << "No path found." << std::endl;
        return;
    }

    std::cout << "Best path length: " << bestPathLength << std::endl;
    std::cout << "Best path visual representation with (0,0) at bottom left:" << std::endl;

    // 创建网格
    std::vector<std::vector<char>> grid(map.getHeight(), std::vector<char>(map.getWidth(), ' '));

    // 标注障碍物
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            if (map.isObstacle(x, y)) {
                grid[map.getHeight() - y - 1][x] = 'X';
            }
        }
    }

    // 标注路径
    for (const auto& node : bestPath) {
        // Assuming y is inverted due to (0,0) being at the bottom left
        grid[map.getHeight() - node.second - 1][node.first] = '*';
    }

    // 画图
    for (int y = 0; y < map.getHeight(); ++y) {
        // 上边界
        if (y == 0) {
            for (int x = 0; x < map.getWidth(); ++x) {
                std::cout << "+---";
            }
            std::cout << "+" << std::endl;
        }

        for (int x = 0; x < map.getWidth(); ++x) {
            // 左边界
            std::cout << "| " << grid[y][x]<<" ";
        }
        // 右边界
        std::cout << "|" << std::endl;

        // 下边界
        for (int x = 0; x < map.getWidth(); ++x) {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;
    }
}

void AntColony::printPheromones() const {
    for (int y = pheromones.size() - 1; y >= 0; --y) {
        // 上边界
        if (y == pheromones.size() - 1) {
            std::cout << "+";
            for (int x = 0; x < pheromones[y].size(); ++x) {
                std::cout << "---+";
            }
            std::cout << std::endl;
        }

        // 信息素值和左右边界
        for (int x = 0; x < pheromones[y].size(); ++x) {
            if (x == 0) std::cout << "|";
            // 显示信息素值的整数部分，每个数字占据三个字符的宽度，大于999显示999
            if (pheromones[x][y] < 999)
                std::cout << std::setw(3) << std::setfill(' ') << static_cast<int>(pheromones[x][y]) << "|";
            else
                std::cout << std::setw(3) << std::setfill(' ') << 999 << "|";
        }
        std::cout << std::endl;

        // 下边界
        std::cout << "+";
        for (int x = 0; x < pheromones[y].size(); ++x) {
            std::cout << "---+";
        }
        std::cout << std::endl;
    }
}

void AntColony::setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void AntColony::progressBar(int current, int total, const std::string& prefix, int barWidth, int posY) {
    float progress = (float)current / total;
    int pos = static_cast<int>(barWidth * progress);

    setCursorPosition(0, posY);
    std::cout << prefix << " [";

    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << current << "/" << total << "   "; // 多输出几个空格以覆盖旧的文本
}

void AntColony::savePheromoneMatrixToCSV(const std::vector<std::vector<double>>& pheromones, int iteration) {
    std::string filename = "pheromones_" + std::to_string(iteration) + ".csv";
    std::ofstream file(filename);

    for (int y = 0; y < pheromones.size(); ++y) {
        for (int x = 0; x < pheromones[y].size(); ++x) {
            file << pheromones[y][x];
            if (x < pheromones[y].size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

