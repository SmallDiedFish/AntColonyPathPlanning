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
#pragma once
#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include "GridMap.h"
#include <utility>
#include <vector>
#include <windows.h>
#include <string>
#include <iomanip>
#include <fstream>

struct Ant {
    std::vector<std::pair<int, int>> path; // �����߹���·�����洢�����
    double pathLength; // �����߹���·������
    bool arrived = false;
    std::pair<int, int> currentPosition; // ���ϵĵ�ǰλ��
    Ant(std::pair<int, int> start) : currentPosition(start), pathLength(0) {
        path.push_back(start);
    }
};

class AntColony {
public:
    AntColony(GridMap& map, int antCount, int maxIterations, std::pair<int, int> start, std::pair<int, int> end);
    void run();
    void printBestPath() const; // ��ӡ�ҵ������·��
    void printPheromones() const;

private:
    GridMap& map;
    std::vector<Ant> ants;
    std::vector<std::vector<double>> pheromones; // ��Ϣ�ؾ���
    std::pair<int, int> start, end;
    int antCount; //��������
    int maxIterations; // ����������
    const double alpha = 1.0; // ��Ϣ�ص������Ҫ��
    const double beta = 3.0; // ����ʽ���ӵ������Ҫ��
    const double evaporationRate = 0.3; // ��Ϣ�ص�������
    const double Q = 100; // ��Ϣ��ǿ�ȳ���
    void initializeAnts();
    void initializePheromones();
    void moveAnts();
    void updatePheromones();
    int calculatePathLength(const std::vector<std::pair<int, int>>& path) const;
    int chooseNextNode(const Ant& ant);
    void evaluateAndUpdateBestPath(Ant& ant);
    void printAntsDistribution() const;
    void progressBar(int currentValue, int maximumValue, const std::string& prefix);
    int getMaxSteps() const;
    std::vector<std::pair<int, int>> getFeasibleNextNodes(const Ant& ant) const;
    std::vector<std::pair<int, int>> bestPath; 
    int bestPathLength = 9999999; 
    void setCursorPosition(int x, int y);
    void progressBar(int current, int total, const std::string& prefix, int barWidth, int posY);
    void savePheromoneMatrixToCSV(const std::vector<std::vector<double>>& pheromones, int iteration);
};

#endif
