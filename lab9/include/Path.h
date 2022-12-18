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
#include <stack>

using namespace std;

template<class T>
bool Contains(std::vector<T>& v, T value)
{
    return (std::find(v.begin(), v.end(), value) != v.end());
}

std::tuple<int, int, std::vector<int>> hamiltonian_path(vector<vector<int>>& adj_mtx)
{
    ///�������� ��������� �������
    int starter_vertex = rand() % adj_mtx.size();
    int final_vertex;
    ///��������� �������� �������
    do
    {
        final_vertex = rand() % adj_mtx.size();
    } while (starter_vertex == final_vertex);
    /// ������ ������ ������ ����(result)
    std::vector<int> path{starter_vertex};
    std::vector<std::vector<int>> visited_vert(adj_mtx.size(), std::vector<int>(adj_mtx.size(), 0));
    /// �� �������� ������� ����� ���
    for(auto& item: visited_vert[final_vertex]) item = 1;
    while (!path.empty())
    {
        bool move_deep = false;
        int curr_vert = path[path.size() - 1]; /// ������� �������
        for (int i = 0; i < adj_mtx.size(); i++)
            if ((adj_mtx[curr_vert][i] == 1) /// ����� ���� �������
            && visited_vert[curr_vert][i] == 0) /// �� �� �������� ��� �������
                if (not Contains(path, i)) /// ���� ���� �� �������� �������
                {
                    visited_vert[curr_vert][i] = 1; /// ��������� ��� ����������
                    path.push_back(i); /// ��������� � ���� �������
                    move_deep = true; /// �� ��������� ������� ��� ��������
                    if ((path.size() == adj_mtx.size()) /// ���� ���-�� ��������� � ���� ����� ���-�� ������
                    && (path[path.size() - 1] == final_vertex)) /// ���� ��������� ������� ��� final_vertex
                        return std::make_tuple(starter_vertex, final_vertex, path);
                    break;
                }
        if (!move_deep) /// ���� �� �� ������ ��������� �������
            path.pop_back();/// �� ���������� ���� �� ����� ����
    }
    throw std::string("hamilton path not found");
}