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

using namespace std;


///Поиск гамильтонова цикла методом ветвей и границ в списке вершин
class BranchAndBoundAdjMtx
{
public:
    /// Основной метод
    std::vector<int> Calc(vector<vector<int>>& adj_mtx)
    {
        /// Инициализация полей
        vector<int> current_path(adj_mtx.size() + 1, -1);
        visited = std::vector<bool>(adj_mtx.size(), false);
        final_path = std::vector<int>(adj_mtx.size() + 1, -1);
        /// Выбор начальной вершины
        int starter_vertex = rand() % adj_mtx.size();
        visited[starter_vertex] = true;
        current_path[0] = starter_vertex;

        int current_bound = 0;
        TSPRec(adj_mtx, current_bound, 0, 1, current_path);
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
    int firstMin(vector<vector<int>>& adj_mtx, int i)
    {
        int min = max_value;
        for (int k = 0; k < adj_mtx.size(); k++)
            if (adj_mtx[i][k] < min && i != k)
                min = adj_mtx[i][k];
        return min;
    }

    /// function to find the second minimum edge cost
    /// having an end at the vertex i
    int secondMin(vector<vector<int>>& adj_mtx, int i)
    {
        int first = max_value, second = max_value;
        for (int j = 0; j < adj_mtx.size(); j++) {
            if (i == j)
                continue;

            if (adj_mtx[i][j] <= first)
            {
                second = first;
                first = adj_mtx[i][j];
            }
            else if (adj_mtx[i][j] <= second && adj_mtx[i][j] != first)
                second = adj_mtx[i][j];
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
    void TSPRec(vector<vector<int>>& adj_mtx, int cur_bound, int cur_weight, int level, vector<int>& cur_path)
    {
        /// Когда уровень достигнет кол-ва вершин
        if (level == adj_mtx.size())
        {
            /// check if there is an edge from last vertex in
            /// path back to the first vertex
            if (adj_mtx[cur_path[level - 1]][cur_path[0]] != 0)
            {
                /// curr_res has the total weight of the
                /// solution we got
                int current_res = cur_weight + adj_mtx[cur_path[level - 1]][cur_path[0]];
                /// Update final result and final path if
                /// current result is better.
                if (current_res < final_res)
                {
                    copyToFinal(cur_path);
                    final_res = current_res;
                }
            }
            return;
        }
        /// for any other level iterate for all vertices to
        /// build the search space tree recursively
        for (int i = 0; i < adj_mtx.size(); i++)
        {
            /// Consider next vertex if it is not same (diagonal
            /// entry in adjacency matrix and not visited
            /// already)
            if (adj_mtx[cur_path[level - 1]][i] != 0 && visited[i] == false)
            {
                int temp = cur_bound;
                cur_weight += adj_mtx[cur_path[level - 1]][i];
                /// different computation of curr_bound for
                /// level 2 from the other levels
                cur_bound -= firstMin(adj_mtx, i) / 2;
                if (level == 1)
                    cur_bound -= firstMin(adj_mtx, cur_path[level - 1]);
                else
                    cur_bound -= secondMin(adj_mtx, cur_path[level - 1]);

                /// curr_bound + curr_weight is the actual lower bound
                /// for the node that we have arrived on
                /// If current lower bound < final_res, we need to explore
                /// the node further
                if (cur_bound + cur_weight < final_res)
                {
                    cur_path[level] = i;
                    visited[i] = true;
                    /// call TSPRec for the next level
                    TSPRec(adj_mtx, cur_bound, cur_weight, level + 1, cur_path);
                }

                /// Else we have to prune the node by resetting
                /// all changes to curr_weight and curr_bound
                cur_weight -= adj_mtx[cur_path[level - 1]][i];
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
