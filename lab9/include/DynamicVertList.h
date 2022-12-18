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

class DynamicVertList {
public:
    std::vector<int> Calc(map<int, map<int, int>> &vert_list)
    {
        int max_value = 1e5;
        ///������������ �����
        int max_mask = (1 << vert_list.size());

        ///��������� ������ ��������� inf
        vector<vector<int>> distance(vert_list.size(), vector<int>(max_mask, max_value));
        int starter_vertex = rand() % vert_list.size();
        distance[starter_vertex][0] = 0;

        ///���� ���������� � ���������
        int path = find_path(vert_list, distance, starter_vertex, ((1 << vert_list.size()) - 1));

        ///����� ����
        vector<int> way = find_way(vert_list, distance, starter_vertex);
        return way;
    }
private:
    vector<int> find_way(map<int, map<int, int>> &vert_list, vector<vector<int>> &dist, int starter_vertex)
    {
        ///������
        int i = starter_vertex;
        /// ������������ �����
        int mask = (1 << vert_list.size()) - 1;
        vector<int> path{i};
        while (mask != 0) /// �� ��� ��� ���� �� ����� ��������� ��� �������
        {
            for (auto vertex: vert_list[i + 1]) {
                int j = vertex.first - 1;
                int W =  vert_list[i + 1][j + 1];
                if ((mask & (1 << j)) /// ������� j �� ���������
                && (dist[i][mask] == dist[j][mask - (1 << j)] + W))
                {
                    path.push_back(j);      /// ��������� ������� � ����
                    i = j;                  /// ������ ������� �� ������� ����
                    mask = mask - (1 << j); /// �������� ������� j ��� ����������
                    continue;
                }
            }
        }
        return path;
    }

    int find_path(map<int, map<int, int>> &vert_list, vector<vector<int>> &dist, int i, int mask)
    {
        int max_value = 1e5;
        /// ���� ������� ��� �������
        if (dist[i][mask] != max_value)
            return dist[i][mask];

        for (auto vertex: vert_list[i + 1])
        {
            int j = vertex.first - 1;
            if (mask & (1 << j)) /// ���� ������� j �� ���������
            {
                ///���������� �������� � ������� ������, �� ��� ������� j
                int path = find_path(vert_list, dist, j, mask - (1 << j));
                ///����������� ���� ����������� ����
                dist[i][mask] = min(dist[i][mask], path + vert_list[i + 1][j + 1]);
            }
        }
        return dist[i][mask];
    }
};