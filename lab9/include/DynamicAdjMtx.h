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

int max_value = 1e5;

class DynamicAdjMtx
{
public:
    std::vector<int> Calc(vector<vector<int>>& adj_mtx)
    {
        ///������������ �����
        int max_mask = (1 << adj_mtx.size());
        ///��������� ������ ��������� inf
        vector<vector<int>> distance(adj_mtx.size(), vector<int>(max_mask, max_value));
        int starter_vertex = rand() % adj_mtx.size();
        distance[starter_vertex][0] = 0;
        ///���� ���������� � ���������
        int path = find_path(adj_mtx, distance, starter_vertex, max_mask - 1);
        ///����� ����
        vector<int> way = find_way(adj_mtx, distance, starter_vertex);
        return way;
    }
private:
    int find_path(vector<vector<int>>& adj_mtx, vector<vector<int>>& dist, int i, int mask)
    {
        /// ���� ������� ��� �������
        if (dist[i][mask] != max_value)
            return dist[i][mask];
        for (int j = 0; j < adj_mtx.size(); j++)
        {
            if ((adj_mtx[i][j] != 0) /// ����� ��� ������
                && (adj_mtx[i][j] != max_value) /// ����� ��� �� ����� inf
                && (mask & (1 << j))) /// ���� j ������� �� ���������
            {
                ///���������� �������� � ������� ������, �� ��� ������� j
                int path = find_path(adj_mtx, dist, j, mask - (1 << j));
                ///����������� ���� ����������� ����
                dist[i][mask] = min(dist[i][mask], path + adj_mtx[i][j]);
            }
        }
        return dist[i][mask];
    }

    vector<int> find_way(vector<vector<int>>& adj_mtx, vector<vector<int>>& dist, int starter_vertex)
    {
        ///������
        int i = starter_vertex;
        /// ������������ �����
        int mask = (1 << adj_mtx.size()) - 1;
        vector<int> path{i};
        while (mask != 0) /// �� ��� ��� ���� �� ����� ��������� ��� �������
            for (int j = 0; j < adj_mtx.size(); j++)
            {
                int W = adj_mtx[i][j];
                if ((adj_mtx[i][j] != 0) /// ����� ��� ������
                    && (mask & (1 << j)) /// ������� j �� ���������
                    ///  d[i][mask] = (d[j][invMask] + W)
                    /// where mask = 111,   invMask = 110 if j = 0
                    && (dist[i][mask] == (dist[j][mask - (1 << j)] + W)))
                {
                    path.push_back(j);      /// ��������� ������� � ����
                    i = j;                  /// ������ ������� �� ������� ����
                    mask = mask - (1 << j); /// �������� ������� j ��� ����������
                    continue;
                }
            }
        return path;
    }
};