//
// Created by Азим on 18.12.2022.
//

#ifndef GENERATOR_GENERATION_H
#define GENERATOR_GENERATION_H
#include "DynamicAdjMtx.h"
#include "DynamicVertList.h"
#include "BranchAndBoundAdjMtx.h"
#include "BranchAndBoundVertList.h"
#include "Cycle.h"
#include "Path.h"
#include "Genetic.h"
using namespace std;

/// <summary>
/// Матрица связности
/// </summary>
/// <param name="vertexes_number"></param>
/// <param name="edges_number"></param>
/// <returns></returns>
vector<vector<int>> adjacency_matrix_new(int vertexes_number, int edges_number) {
    srand(time(NULL));
    vector<vector<int>> matrix;

    vector<int> line(vertexes_number, 0);
    for (int i = 0; i < vertexes_number; i++) {
        matrix.push_back(line);
    }
    vector<int> remained_vertexes;
    for (int i = 0; i < vertexes_number; i++) {
        remained_vertexes.push_back(i + 1);
    }
    vector<int> connected_vertexes;

    int chosen_index = rand() % remained_vertexes.size();
    int random_left_vertex = remained_vertexes[chosen_index];
    connected_vertexes.push_back(random_left_vertex);
    remained_vertexes.erase(remained_vertexes.begin() + chosen_index);
    int random_right_vertex;
    while (remained_vertexes.size() != 0) {
        chosen_index = rand() % remained_vertexes.size();
        random_right_vertex = remained_vertexes[chosen_index];
        connected_vertexes.push_back(random_right_vertex);
        remained_vertexes.erase(remained_vertexes.begin() + chosen_index);
        matrix[random_left_vertex - 1][random_right_vertex - 1] = 1;
        matrix[random_right_vertex - 1][random_left_vertex - 1] = 1;
        chosen_index = rand() % connected_vertexes.size();
        int random_left_vertex = connected_vertexes[chosen_index];
    }
    int current_edges_number = vertexes_number - 1;
    while (current_edges_number < edges_number) {
        chosen_index = rand() % connected_vertexes.size();
        random_left_vertex = connected_vertexes[chosen_index];
        do {
            chosen_index = rand() % connected_vertexes.size();
            random_right_vertex = connected_vertexes[chosen_index];
        } while (random_left_vertex == random_right_vertex);
        if (matrix[random_left_vertex - 1][random_right_vertex - 1] != 0) {
            continue;
        }
        matrix[random_left_vertex - 1][random_right_vertex - 1] = 1;
        matrix[random_right_vertex - 1][random_left_vertex - 1] = 1;
        current_edges_number++;
    }
    return matrix;
}
/// <summary>
///  Вывод матрицы
/// </summary>
/// <param name="matrix"></param>
/// <param name="filename"></param>
void print(ofstream& buf ,vector<vector<int>>& matrix) {
    buf << ";;";
    for (int i = 0; i < matrix.size(); i++)
        buf << i << ";";
    buf << "\n";
    for (int i = 0; i < matrix.size(); i++)
    {
        buf << ";" << i <<";";
        for (int j = 0; j < matrix[i].size(); j++)
            buf << matrix[i][j] << ";";
        buf << "\n";
    }
}

/// <summary>
/// Проверка на гамильтонов цикл
/// </summary>
/// <param name="adjacency_matrix"></param>
/// <returns></returns>
bool is_hamiltonian(vector<vector<int>>& adjacency_matrix)
{
    int min_neighs = adjacency_matrix.size() / 2;
    if (adjacency_matrix.size() % 2 != 0)
        min_neighs++;
    for (int i = 0; i < adjacency_matrix.size(); i++)
    {
        int neighs = 0;
        for (int j = 0; j < adjacency_matrix.size(); j++)
            if (adjacency_matrix[i][j] == 1)
                neighs++;
        if (neighs < min_neighs)
            return false;
    }
    return true;
}

/// Генерация гамильтонова графа
vector<vector<int>> hamiltonian_graph_new(vector<vector<int>>& adjacency_matrix, int edges_number)
{
    int vertexes_number = adjacency_matrix.size();
    int edges_number_max = vertexes_number * (vertexes_number - 1) / 2;
    vector<vector<int>> new_matrix = adjacency_matrix;
    while (!is_hamiltonian(new_matrix))
    {
        if (edges_number < edges_number_max)
            edges_number++;
        new_matrix = adjacency_matrix_new(vertexes_number, edges_number);
    }
    return new_matrix;
}

/// <summary>
/// Заполнение весами
/// </summary>
/// <param name="adjacency_matrix"></param>
void fill_weights(vector<vector<int>>& adjacency_matrix)
{
    srand(time(NULL));
    int minimal_weight = 1, maximal_weight = 30;

    for (int i = 0; i < adjacency_matrix.size(); i++)
        for (int j = i + 1; j < adjacency_matrix.size(); j++)
        {
            int random_length = minimal_weight + rand() % (maximal_weight - minimal_weight + 1);

            if (adjacency_matrix[i][j] != 0)
                adjacency_matrix[i][j] = adjacency_matrix[j][i] = random_length;
            else
                adjacency_matrix[j][i] = adjacency_matrix[i][j] = max_value;
        }
}

/// <summary>
/// Вывод матрицы весов
/// </summary>
/// <param name="matrix"></param>
void print_weight(ofstream& stream ,vector<vector<int>>& matrix) {
    stream << ";;";
    for (int i = 0; i < matrix.size(); i++)
        stream << i << ";";
    stream << "\n";
    for (int i = 0; i < matrix.size(); i++)
    {
        stream << ";" << i << ";";
        for (int j = 0; j < matrix[i].size(); j++)
            if (matrix[i][j] == max_value)
                stream << "0;";
            else
                stream << matrix[i][j]<<";";
        stream << ";";
        stream << "\n";
    }
}


/// <summary>
/// Конвертор в список вершин
/// </summary>
/// <param name="adjacency_matrix"></param>
/// <returns></returns>
map<int, map<int, int>> convert_to_vertexes_list(vector<vector<int>>& adjacency_matrix) {
    map<int, map<int, int>> vertexes_list;
    for (int i = 0; i < adjacency_matrix.size(); i++) {
        map<int, int> vertex_and_length;
        for (int j = 0; j < adjacency_matrix.size(); j++)
            if ((adjacency_matrix[i][j] != 0) && (adjacency_matrix[i][j] != max_value))
                vertex_and_length.insert({ j + 1, adjacency_matrix[i][j] });
        vertexes_list.insert({ i + 1, vertex_and_length });
    }
    return vertexes_list;
}
#endif //GENERATOR_GENERATION_H
