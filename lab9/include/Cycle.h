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

void swap_cycle(deque<int>& cycle, int start_index, int end_index)
{
    int middle = (end_index - start_index + 1) / 2;
    for (int i = start_index; i < middle; i++)
        std::swap(cycle[i], cycle[end_index - i]);
}

/// <summary>
/// Гамильтонов цикл
/// </summary>
/// <param name="adj_mtx"></param>
vector<int> hamiltonian_cycle(vector<vector<int>>& adj_mtx) {
    ///Выбираем случайную величину
    int starter_vertex = rand() % adj_mtx.size();
    deque<int> cycle{starter_vertex};
    ///Заполняем цикл вершин
    for (int i = 0; i < adj_mtx.size(); i++)
        if (i != starter_vertex)
            cycle.push_back(i);

    int size = adj_mtx.size() * (adj_mtx.size() - 1);
    for (int iter = 0; iter < size; iter++)
    {
        if (adj_mtx[cycle[0]][cycle[1]] == 0)/// Если между смежными вершинами нет связей
        {
            int good_index = 2;
            /// Ищем промежуточную вершину между cycle[0] и cycle[1]
            while ((adj_mtx[cycle[0]][cycle[good_index]] == 0)
                || (adj_mtx[cycle[1]][cycle[good_index + 1]] == 0))
            {
                good_index++;
            }
            /// Зеркальные смещения цикла от 1 до good_index / 2;
            swap_cycle(cycle, 1, good_index);
        }
        /// Перетасовываем
        cycle.push_back(cycle[0]);
        cycle.pop_front();
    }
    ///Добавляем в конец начальную вершину
    cycle.push_back(cycle[0]);
    ///Преобразование в vector
    std::vector<int> result;
    for (auto item: cycle)
        result.push_back(item);

    return result;
}
