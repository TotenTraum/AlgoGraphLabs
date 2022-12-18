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
    ///Выбирает стартовую вершину
    int starter_vertex = rand() % adj_mtx.size();
    int final_vertex;
    ///Подбираем конечную вершину
    do
    {
        final_vertex = rand() % adj_mtx.size();
    } while (starter_vertex == final_vertex);
    /// Список вершин нашего пути(result)
    std::vector<int> path{starter_vertex};
    std::vector<std::vector<int>> visited_vert(adj_mtx.size(), std::vector<int>(adj_mtx.size(), 0));
    /// Из конечной вершины дорог нет
    for(auto& item: visited_vert[final_vertex]) item = 1;
    while (!path.empty())
    {
        bool move_deep = false;
        int curr_vert = path[path.size() - 1]; /// Текущая вершина
        for (int i = 0; i < adj_mtx.size(); i++)
            if ((adj_mtx[curr_vert][i] == 1) /// Такой путь имеется
            && visited_vert[curr_vert][i] == 0) /// Мы не посещали эту вершину
                if (not Contains(path, i)) /// Если путь не содершит вершину
                {
                    visited_vert[curr_vert][i] = 1; /// Указываем как посещенную
                    path.push_back(i); /// Добавляем в путь вершину
                    move_deep = true; /// Мы подобрали вершину для маршрута
                    if ((path.size() == adj_mtx.size()) /// Если кол-во элементов в пути равно кол-ву вершин
                    && (path[path.size() - 1] == final_vertex)) /// Если последний элемент это final_vertex
                        return std::make_tuple(starter_vertex, final_vertex, path);
                    break;
                }
        if (!move_deep) /// Если мы не смогли подобрать вершину
            path.pop_back();/// Не продолжаем идти по этому пути
    }
    throw std::string("hamilton path not found");
}