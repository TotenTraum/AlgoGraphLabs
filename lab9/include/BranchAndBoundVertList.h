#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <math.h>
#include "BranchAndBoundAdjMtx.h"

using namespace std;

///Поиск гамильтонова цикла методом ветвей и границ в списке вершин
class BranchAndBoundVertList
{
public:
    /// Основной метод
    std::vector<int> Calc(map<int, map<int, int>>& vertexes_list)
    {
        /// Инициализация полей
        vector<int> current_path(vertexes_list.size() + 1, -1);
        visited = std::vector<bool>(vertexes_list.size(), false);
        final_path = std::vector<int>(vertexes_list.size() + 1, -1);
        /// Выбор начальной вершины
        int starter_vertex = rand() % vertexes_list.size();
        visited[starter_vertex] = true;
        current_path[0] = starter_vertex;

        int current_bound = 0;
        TSPRec(vertexes_list, current_bound, 0, 1, current_path);
        return final_path;
    }
private:
    int max_value = 1e5;
    int final_res = max_value;
    int final_res_list = max_value;
    vector<int> final_path;
    vector<bool> visited;

    /// Function to copy temporary solution to
    /// the final solution
    void copyToFinal(vector<int>& current_path)
    {
        for (int i = 0; i < current_path.size() - 1; i++)
            final_path[i] = current_path[i];
        final_path[current_path.size() - 1] = current_path[0];
    }

    /// Function to find the minimum edge cost
    /// having an end at the vertex i
    int firstMin(map<int, map<int, int>>& vertexes_list, int i)
    {
        int min = max_value;
        for (auto vertex : vertexes_list[i + 1])
        {
            int k = vertex.first - 1;
            if (vertexes_list[i + 1][k + 1] < min && i != k)
                min = vertexes_list[i + 1][k + 1];
        }
        return min;
    }

    /// function to find the second minimum edge cost
    /// having an end at the vertex i
    int secondMin(map<int, map<int, int>>& vert_list, int i)
    {
        int first = max_value, second = max_value;
        for (auto vertex : vert_list[i + 1]) {
            int j = vertex.first - 1;
            if (i == j)
                continue;

            if (vert_list[i + 1][j + 1] <= first) {
                second = first;
                first = vert_list[i + 1][j + 1];
            }
            else if (vert_list[i + 1][j + 1] <= second
                     && vert_list[i + 1][j + 1] != first)
                second = vert_list[i + 1][j + 1];
        }
        return second;
    }

    /// function that takes as arguments:
    /// curr_bound -> lower bound of the root node
    /// curr_weight-> stores the weight of the path so far
    /// level-> current level while moving in the search
    ///         space tree
    /// curr_path[] -> where the solution is being stored which
    ///                would later be copied to final_path[]
    void TSPRec(map<int, map<int, int>>& vert_list, int cur_bound, int cur_weight, int level, vector<int>& cur_path)
    {
        /// Когда уровень достигнет кол-ва вершин
        if (level == vert_list.size())
        {
            /// check if there is an edge from last vertex in
            /// path back to the first vertex
            if (vert_list[cur_path[level - 1] + 1][cur_path[0] + 1] != 0)
            {
                /// curr_res has the total weight of the
                /// solution we got
                int current_res = cur_weight + vert_list[cur_path[level - 1] + 1][cur_path[0] + 1];
                if (current_res < final_res_list)
                {
                    /// Update final result and final path if
                    /// current result is better.
                    copyToFinal(cur_path);
                    final_res_list = current_res;
                }
            }
            return;
        }

        /// for any other level iterate for all vertices to
        /// build the search space tree recursively
        for (auto vertex : vert_list[cur_path[level - 1] + 1]) {
            int i = vertex.first - 1;
            /// Consider next vertex if it is not same
            if (visited[i] == false)
            {
                int temp = cur_bound;
                cur_weight += vert_list[cur_path[level - 1] + 1][i + 1];
                /// different computation of curr_bound for
                /// level 2 from the other levels
                cur_bound -= firstMin(vert_list, i) / 2;
                if (level == 1)
                    cur_bound -= firstMin(vert_list, cur_path[level - 1]);
                else
                    cur_bound -= secondMin(vert_list, cur_path[level - 1]);

                /// curr_bound + curr_weight is the actual lower bound
                /// for the node that we have arrived on
                /// If current lower bound < final_res, we need to explore
                /// the node further
                if (cur_bound + cur_weight < final_res)
                {
                    cur_path[level] = i;
                    visited[i] = true;
                    /// call TSPRec for the next level
                    TSPRec(vert_list, cur_bound, cur_weight, level + 1, cur_path);
                }

                /// Else we have to prune the node by resetting
                /// all changes to curr_weight and curr_bound
                cur_weight -= vert_list[cur_path[level - 1] + 1][i + 1];
                cur_bound = temp;

                /// Also reset the visited array
                for (int i = 0; i < visited.size(); i++)
                    visited[i] = false;
                for (int j = 0; j <= level - 1; j++)
                    visited[cur_path[j]] = true;
            }
        }
    }
};
