//
// Created by Азим on 20.09.2022.
//

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <functional>
#include <queue>
#include <stack>
#include "View2.h"
#include "View2-Linked.h"
#include "LinkedDirectedGraph.h"
#include "View4.h"

void DFS(IView2& w,int curVertex, std::vector<bool>& vertexs,const std::function<void(const std::string&)>& func)
{
    vertexs[curVertex] = true;
    func(std::to_string(curVertex));
    for(int i = 0; i < w.size(); i++)
        if(not vertexs[i] && (w[curVertex][i] == 1))
        {
            DFS(w,i,vertexs, func);

        }
}

std::vector<bool> DFS(IView2& w,int start,std::function<void(const std::string&)> func)
{
    std::vector<bool> vertexs(w.size(),false);
    DFS(w,start,vertexs,func);
    return vertexs;
}

void BFS(View4& w,int curVertex, std::vector<bool>& vertexs,const std::function<void(const std::string&)>& func)
{
    std::deque<int> queue;
    queue.push_back(curVertex);
    while(not queue.empty())
    {
        int vertex = queue.front();
        queue.pop_front();
        if(vertexs[vertex] != true)
        {
            vertexs[vertex] = true;
            func(std::to_string(vertex));
            for(int i = 0; i < w.links.size();i++)
                if(w[vertex][i] == 1)
                {
                    int pair = (w.links[i].first == vertex) ? w.links[i].second : w.links[i].first;
                    if(vertexs[pair] != true)
                    {
                        queue.push_back(pair);
                    }
                }
        }
    }
}

std::vector<bool> BFS(View4& w,int start,std::function<void(const std::string&)> func)
{
    std::vector<bool> vertexs(w.size(),false);
    BFS(w,start,vertexs,func);
    return vertexs;
}

void BFS(IView2& w,int curVertex, std::vector<bool>& vertexs,const std::function<void(const std::string&)>& func)
{
    std::deque<int> queue;
    queue.push_back(curVertex);
    while(not queue.empty())
    {
        int vertex = queue.front();
        queue.pop_front();
        if(vertexs[vertex] != true)
        {
            vertexs[vertex] = true;
            func(std::to_string(vertex));
            for(int i = 0; i < w.size();i++)
                if(w[vertex][i] == 1)
                {
                    if(vertexs[i] != true)
                    {
                        queue.push_back(i);
                    }
                }
        }
    }
}

std::vector<bool> BFS(IView2& w,int start,std::function<void(const std::string&)> func)
{
    std::vector<bool> vertexs(w.size(),false);
    BFS(w,start,vertexs,func);
    return vertexs;
}

auto GetReachabilityMatrix(IView2& w)
{
    std::vector<std::vector<bool>> matrix;
    for(int i = 0; i < w.size(); i++)
        matrix.push_back(BFS(w,i,[](const std::string&){}));

    return matrix;
}

auto GetCounterReachabilityMatrix(IView2& w)
{
    std::vector<std::vector<bool>> matrix;
    for(int i = 0; i < w.size(); i++)
        matrix.emplace_back(w.size());

    for(int i = 0; i < w.size(); i++)
    {
        auto val = BFS(w,i,[](const std::string&){});
        for(int j = 0; j < w.size(); j++)
            matrix[j][i] = val[j];
    }
    return matrix;
}

auto GetStrongConnectedComponentMatrix(const std::vector<std::vector<bool>>& reach,
                                       const std::vector<std::vector<bool>>& counterReach)
{
    auto mtx = counterReach;
    for(int i = 0; i < counterReach.size(); i++)
        for(int j = 0; j < counterReach.size(); j++)
            mtx[i][j] = mtx[i][j] && reach[i][j];
    return mtx;
}

auto GetStrongConnectedComponents(const std::vector<std::vector<bool>>& SCCMtx)
{
    std::vector<std::vector<int>> components;
    for(auto& row : SCCMtx)
    {
        std::vector<int> newRow;
        for(int i = 0; i < row.size(); i++)
            if(row[i] == true)
                newRow.emplace_back(i);
        if(not std::any_of(components.begin(), components.end(),
                       [&newRow](auto& item){return item == newRow;}))
            components.push_back(newRow);
    }
    return components;
}

auto GetReachabilityMatrixFloyd(IView2& w)
{
    std::vector<std::vector<int>> matrix = w;
    for(int k = 0; k < matrix.size(); k++)
        for(int i = 0; i < matrix.size(); i++)
            for(int j = 0; j < matrix.size(); j++)
                matrix[i][j] = matrix[i][j] || (matrix[i][k] && matrix[k][j]);
    for(int i = 0; i < matrix.size(); i++)
            matrix[i][i] = 1;
    return matrix;
}

auto GetReachabilityMatrixDFS(IView2& w)
{
    std::vector<std::vector<bool>> matrix;
    for(int i = 0; i < w.size(); i++)
        matrix.push_back(DFS(w,i,[](const std::string&){}));

    return matrix;
}

IView2* ConvertToNotDirGraph(const IView2& graph)
{
    IView2* view = new View2_Linked(graph.size());
    for(int i = 0; i < graph.size(); i++)
    {
        for (int j = i + 1; j < graph.size(); j++)
        {
            (*view)[j][i] = (*view)[i][j] = graph[i][j];
        }
    }

    std::srand(time(nullptr));
    int index = std::rand() % graph.size();
    for(int i = 0; i < graph.size(); i++)
    {
        (*view)[index][i] = 0;
        (*view)[i][index] = 0;
    }

    return view;
}

///Удаляет из множества элемент, если он присутствует в нем
template<class T>
void eraseIfHave(std::set<T>& set,const T& val)
{
    auto theIterator= set.find(val);
    if (theIterator!=set.end())
        set.erase(theIterator);
}

/// Выкидывает из стека значение
template<class T>
T Pop(std::stack<T>& stack)
{
    T val = stack.top();
    stack.pop();
    return val;
}

auto GetConnectedComponents(IView2& mtx)
{
    std::vector<std::vector<int>> components;
    for(int i = 0; i < mtx.size(); i++)
    {
        std::vector<int> newRow;
        auto val = BFS(mtx,i,[](const std::string&){});
        for(int j=0;j < val.size();j++)
            if(val[j] == 1)
                newRow.emplace_back(j);
        if(not std::any_of(components.begin(), components.end(),
                           [&newRow](auto& item){return item == newRow;}))
            components.push_back(newRow);
    }
    return components;
}

/// \name Алгоритм Брона-Кербоша на поиск максимальных клик
/// \refitem https://medium.com/nuances-of-programming/%D0%B3%D1%80%D0%B0%D1%84%D1%8B-%D0%B8-%D0%BF%D1%83%D1%82%D0%B8-%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC-%D0%B1%D1%80%D0%BE%D0%BD%D0%B0-%D0%BA%D0%B5%D1%80%D0%B1%D0%BE%D1%88%D0%B0-%D0%BC%D0%B0%D0%BA%D1%81%D0%B8%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D1%8B%D0%B5-%D0%BA%D0%BB%D0%B8%D0%BA%D0%B8-857d7b7e829d
/// \param matrix
/// \return список максимальных клик
std::list<std::set<int>> kerbosh(IView2& matrix)
{
    /// K - множество возможных вершин максимальной клики
    /// M - множество вершин максимальной клики
    /// P - множество исключенных вершин
    std::set<int> M,K,P;
    std::list<std::set<int>> result;
    std::stack<int> StackOfVert;
    std::stack<std::set<int>> StackOfSets;
    int v;

    ///Заполняем множество возможных вершин максимальной клики всеми вершинами
    for (int i=0; i < matrix.size();i++)
        K.insert(i);
    while (not K.empty() || not M.empty())
    {
        if (not K.empty())
        {

            v=*K.begin();
            ///Сохраняем состояния множеств в стек
            StackOfSets.push(M), StackOfSets.push(K), StackOfSets.push(P);
            ///Сохраняем текущую вершину в стек
            StackOfVert.push(v);
            ///Вставляем текущую вершину в максимальное множество вершин
            M.insert(v);
            ///Удаляем связи вершины v из множеств возможных и исключенных вершин
            for (int i = 0; i < matrix.size(); i++)
                if (matrix[v][i])
                    eraseIfHave(K, i), eraseIfHave(P, i);
            ///Удаляем текущую вершину
            eraseIfHave(K, v);
        }
        else
        {
            ///Если множество исключенных вершин не пусто
            if (P.empty())
                result.push_back(M);/// То сохраняем максимальную клику

            ///Откат к предыдущей вершине
            v = Pop(StackOfVert);
            ///Откат к предыдущим состояниям множеств
            P = Pop(StackOfSets);
            K = Pop(StackOfSets);
            M = Pop(StackOfSets);
            ///Выкидываем вершину из множества возможных вершин в множество исключённых вершин
            eraseIfHave(K, v);
            P.insert(v);
        }
    }
    return result;
}

/// \name Жадный алгоритм поиска минимального доминирующего множества
std::set<int> greedyDominateSetAlgorithm(View2_Linked view)
{
    ///Множество посещённых вершин
    std::set<int> visitedVert;

    ///Функция подсчёта связей в строчках матрицы смежности
    auto CountRows = [](View2_Linked& view) -> std::vector<std::pair<int,int>>
    {
        std::vector<std::pair<int, int>> countRow;
        for(int i=0; i < view.size(); i++)
            countRow.emplace_back(i, std::count_if(view[i].begin(), view[i].end(),[](auto& x){return x == 1;}));
        return countRow;
    };

    ///Функция удаления связей из строчки матрицы смежности
    auto RemoveLinks = [](View2_Linked& view, int row)
    {
        for(int i=0; i < view.size(); i++)
            view[row][i] = view[i][row] = 0;
    };

    ///Функция поиска максимальной строки из результатов f: CountRows
    auto GetMaxRow = [](std::vector<std::pair<int,int>>& pairs) -> int
    {
        if(pairs.empty())
            return -1;
        auto maxPair = pairs[0];
        for (auto& pair: pairs)
            if(pair.second > maxPair.second)
                maxPair = pair;

        if(maxPair.second == 0)
            return -1;
        return maxPair.first;
    };

    auto countedRows = CountRows(view);
    auto maxRow = GetMaxRow(countedRows);
    while (maxRow != -1)
    {
        visitedVert.insert(maxRow);
        RemoveLinks(view, maxRow);
        countedRows = CountRows(view);
        maxRow = GetMaxRow(countedRows);
    }
    return visitedVert;
}

/// Генерирует дополнение графа
void Invert(View2_Linked& view)
{
    for(int i = 0; i < view.size(); i++)
        for(int j = 0; j < view.size(); j++)
            view[i][j] = (view[i][j] == 0 && i != j)? 1: 0;
}

#endif //GRAPH_ALGORITHMS_H
