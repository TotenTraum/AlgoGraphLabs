//
// Created by Азим on 20.09.2022.
//

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <functional>
#include <queue>
#include <stack>
#include <limits>
#include "View2.h"
#include "View2-Linked.h"
#include "LinkedDirectedGraph.h"
#include "View4.h"
#include "EdgesList.h"

using Matrix = std::vector<std::vector<int>>;

////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 2 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////
void DFS(IView2& w,int curVertex, std::vector<bool>& vertexs,const std::function<void(const std::string&)>& func)
{
    vertexs[curVertex] = true;
    func(std::to_string(curVertex));
    for(int i = 0; i < w.size(); i++)
        if(not vertexs[i] && (w[curVertex][i] == 1))
            DFS(w,i,vertexs, func);
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
////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 3 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////
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
        for (int j = i + 1; j < graph.size(); j++)
            (*view)[j][i] = (*view)[i][j] = graph[i][j];

    std::srand(time(nullptr));
    int index = std::rand() % graph.size();
    for(int i = 0; i < graph.size(); i++)
        (*view)[index][i] = (*view)[i][index] = 0;

    return view;
}
////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 4 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////
std::tuple<std::vector<int>, std::vector<int>> BellmanFord(IView2& mtx, int src)
{
    const int inf = std::numeric_limits<int>::max();
    /// Заполнение массива индексов
    std::vector<int> next(mtx.size(), inf);

    /// Заполнение расстояний
    std::vector<int> dist(mtx.size(), inf);
    dist[src] = 0;
    /// Алгоритм поиска пути
    for (int k = 0; k < mtx.size(); ++k)
        for (int i = 0; i < mtx.size(); ++i)
            for (int j = 0; j < mtx.size(); ++j)
            {
                long long w = mtx[i][j];
                if(w != 0)
                    if (dist[i] != inf and dist[i] + w < dist[j])
                    {
                        dist[j] = dist[i] + w;
                        next[j] = i;
                    }
            }
    return std::make_tuple(dist,next);
}

std::vector<int> RestoreBellmanFord(std::vector<int> mtx, int from, int to)
{
    const int inf = std::numeric_limits<int>::max();
    auto path = std::vector<int> {};
    for (auto current = to; current != from && current != inf; current = mtx[current])
        path.push_back(current);
    path.push_back(from);
    std::reverse(path.begin(), path.end());
    return path;
}

std::tuple<Matrix, Matrix> Floyd(IView2& mtx)
{
    const int inf = std::numeric_limits<int>::max();
    /// Заполнение массива индексов
    Matrix next(mtx.size(), std::vector<int>(mtx.size()));
    for (auto i = 0; i < mtx.size(); ++i)
        for (auto j = 0; j < mtx.size(); ++j)
            next[i][j] = j;
    /// Заполнение расстояний
    Matrix dist = mtx;
    for (int i = 0; i < mtx.size(); ++i)
        for (int j = i + 1; j < mtx.size(); ++j)
            if (dist[i][j] == 0)
                dist[i][j] = dist[j][i] = inf;
    /// Алгоритм поиска пути
    for (int k = 0; k < mtx.size(); ++k)
        for (int i = 0; i < mtx.size(); ++i)
            for (int j = 0; j < mtx.size(); ++j)
            {
                long long sum = (long long) dist[i][k] + (long long)dist[k][j];
                if (sum < (long long)dist[i][j]) {
                    dist[i][j] = sum;
                    next[i][j] = next[i][k];
                }
            }
    return std::make_tuple(dist,next);
}

std::vector<int> RestoreFloyd(Matrix mtx, int from, int to)
{
    auto path = std::vector<int> {from};
    while (from != to)
    {
        from = mtx[from][to];
        path.push_back(from);
    }
    return path;
}

int FindNearest(std::vector<int> distance, std::vector<bool> visited)
{
    static int inf = std::numeric_limits<int>::max();
    int minDistance = inf;
    int minIndex = 0;

    for (int i = 0; i < distance.size(); ++i)
        if (!visited[i] && distance[i] <= minDistance)
        {
            minDistance = distance[i];
            minIndex = i;
        }
    return minIndex;
}

std::tuple<std::vector<int>, std::vector<int>> Dikstra(IView2& mtx, int src)
{
    const int inf = std::numeric_limits<int>::max();
    auto visited = std::vector<bool>(mtx.size());
    auto distance = std::vector<int>(mtx.size(), inf);
    auto buildingPaths = std::vector<int>(mtx.size(), inf);
    distance[src] = 0;

    for (int i = 0; i < mtx.size() - 1; ++i)
    {
        int current = FindNearest(distance, visited);
        visited[current] = true;
        for (int j = 0; j < mtx.size(); ++j)
            if (!visited[j] && mtx[current][j] != 0
            && distance[current] != inf
            && distance[current] + mtx[current][j] < distance[j])
            {
                distance[j] = distance[current] + mtx[current][j];
                buildingPaths[j] = current;
            }
    }

    return std::make_tuple(distance, buildingPaths);
}
////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 5 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////

void UniteComponents(std::vector<int>& components, int main, int added)
{
    for (int i = 0; i < components.size(); ++i)
        if (components[i] == added)
            components[i] = main;
}

std::tuple<IView2*,int> BuildKruskal(IView2& graph)
{
    IView2* mst = new View2(graph.size());

    std::vector<int> components(graph.size());
    for (auto i = 0; i < components.size(); ++i) { components[i] = i; }

    auto edges = EdgesList(graph);
    std::sort(edges.begin(), edges.end(),[](const Edge& a,const Edge& b){return a.weight < b.weight;});

    auto weight = 0; // вес мод
    for (auto edge : edges)
    {
        if (components[edge.from] == components[edge.to]) continue;
        (*mst)[edge.from][edge.to] = (*mst)[edge.to][edge.to] = edge.weight;
        weight += edge.weight;
        UniteComponents(components, components[edge.from], components[edge.to]);
    }

    return std::make_tuple(mst, weight);
}

std::tuple<EdgesList,int> BuildKruskal(EdgesList graph)
{
    EdgesList mst{};

    std::vector<int> components(graph.countVertices);
    for (auto i = 0; i < components.size(); ++i) { components[i] = i; }

    auto edges = graph;
    std::sort(edges.begin(), edges.end(),[](const Edge& a,const Edge& b){return a.weight < b.weight;});

    auto weight = 0; // вес мод
    for (auto edge : edges)
    {
        if (components[edge.from] == components[edge.to]) continue;
        mst.push_back(edge);
        weight += edge.weight;
        UniteComponents(components, components[edge.from], components[edge.to]);
    }

    return std::make_tuple(mst, weight);
}

bool IsEdgeUsed(Edge edge, std::vector<bool> visitedVertices)
{
    return !visitedVertices[edge.from] && visitedVertices[edge.to] ||
           !visitedVertices[edge.to] && visitedVertices[edge.from];
}

int FindMinEdgeFromUnused(EdgesList unusedEdges, std::vector<bool> visitedVertices)
{
    int minEdgeIndex = -1;

    for (int i = 0; i < unusedEdges.size(); i++)
    {
        auto currentEdge = unusedEdges[i];
        if (!IsEdgeUsed(currentEdge, visitedVertices)) continue;
        if (minEdgeIndex != -1)
        {
            if (currentEdge.weight < unusedEdges[minEdgeIndex].weight)
                minEdgeIndex = i;
        }
        else
            minEdgeIndex = i;
    }

    return minEdgeIndex;
}

std::tuple<IView2*,int> BuildPrim(IView2& graph)
{
    IView2* mst = new View2(graph.size());
    std::vector<bool> visited(graph.size());
    auto unused = EdgesList(graph);
    visited[0] = true;

    auto weight = 0;
    while (std::any_of(visited.begin(), visited.end(),[](auto a){return a == false;}))
    {
        int minEdgeIndex = FindMinEdgeFromUnused(unused, visited);
        auto minEdge = unused[minEdgeIndex];

        if (visited[minEdge.from]) { visited[minEdge.to] = true; }
        else { visited[minEdge.from] = true; }

        (*mst)[minEdge.from][minEdge.to] = (*mst)[minEdge.to][minEdge.from] = minEdge.weight;
        unused.erase(std::next(unused.begin(),minEdgeIndex));
        weight += minEdge.weight;
    }

    return std::make_tuple(mst, weight);
}

std::tuple<EdgesList,int> BuildPrim(EdgesList graph)
{
    EdgesList mst{};
    std::vector<bool> visited(graph.countVertices);
    auto unused = EdgesList(graph);
    visited[0] = true;

    auto weight = 0;
    while (std::any_of(visited.begin(), visited.end(),[](auto a){return a == false;}))
    {
        int minEdgeIndex = FindMinEdgeFromUnused(unused, visited);
        auto minEdge = unused[minEdgeIndex];

        if (visited[minEdge.from]) { visited[minEdge.to] = true; }
        else { visited[minEdge.from] = true; }

        mst.emplace_back(minEdge.from, minEdge.to, minEdge.weight);
        unused.erase(std::next(unused.begin(),minEdgeIndex));
        weight += minEdge.weight;
    }

    return std::make_tuple(mst, weight);
}

template<class T>
bool Contains(std::vector<T>& v, T& value)
{
    return (std::find(v.begin(), v.end(), value) != v.end());
}

////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 6 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 7 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////

void ColorizeVertex(std::vector<std::vector<int>> graph, std::vector<std::pair<int, int>>& vertices, int numberVertex)
{
    std::vector<int> usedColors; /// Набор уже используемых в раскраске цветов
    std::vector<int> unavailableColors; /// Набор недоступных для раскраски цветов

    for (int i = 0; i < vertices.size(); i++)
    { /// Отбираем набор цветов, которые уже используются (0 не учитывается)
        if (vertices[i].second != 0 && !Contains(usedColors, vertices[i].second))
            usedColors.push_back(vertices[i].second);
    }
    for (int i = 0; i < graph.size(); i++)
    { /// Отбираем цвета, которые мы не можем использовать для закраски текущей вершины
        if (graph[numberVertex][i] == 1 && vertices[i].second != 0 && !Contains(unavailableColors, vertices[i].second))
            /// Цвет соседней вершины - кандидат на роль недоступного для закраски цвета
            unavailableColors.push_back(vertices[i].second);
    }

    std::pair<int, int> tempVertex = std::make_pair(numberVertex, 0); /// Без этого не получится поместить tuple в массив
    if (usedColors.size() == unavailableColors.size())
    { /// Если недоступны все имеющиеся на данный момент цвета
        /// Сортируем, чтобы последним элементом был самый наибольшей по значению цвет
        std::sort(usedColors.begin(), usedColors.end(), [] (int& a, int& b){return a < b; });
        tempVertex.second = usedColors[usedColors.size() - 1] + 1; /// Полседний элемент + 1
    }
    else
    { /// Выбираем любой цвет, который не входит в список недоступных цветов
        int colorVertex = 0;
        for (int i = 0; i < usedColors.size() && colorVertex == 0; i++)
            if (!Contains(unavailableColors, usedColors[i]))
                colorVertex = usedColors[i];
        tempVertex.second = colorVertex;
    }
    vertices[numberVertex] = tempVertex; /// Помещаем результат в массив с вершинами
}

std::vector<std::pair<int, int>> DynamicGraphColorising(View2_Linked& graph)
{
    /// 0 - отсутствие цвета; другие числа - цвет
    std::vector<std::pair<int, int>> result;

    /// Присваиваем всем вершинам цвет 0
    for (int i = 0; i < graph.size(); i++)
        result.push_back(std::pair(i, 0));

    /// Задаем стратовую вершину с цветом 1
    std::pair<int, int> startVertex = std::pair(0, 1);
    result[0] = startVertex;

    /// Раскрашиваем вершины
    for (int i = 1; i < graph.size(); i++)
        ColorizeVertex(graph, result, i);

    return result;
}

template <class T>
std::vector<T> convertSetToVector(std::set<T> s)
{
    std::vector<T> tmp;
    for (auto& item: s)
        tmp.push_back(item);
    return tmp;
}

std::vector<std::pair<int, int>> LeastCoverGraphColorising(View2_Linked& graph)
{
    /// 0 - отсутсвие цвета; другие числа - цвет
    std::vector<std::pair<int, int>> result;

    std::vector<int> greedyDominateSet = convertSetToVector(greedyDominateSetAlgorithm(graph));

    /// Присваиваем всем вершинам цвет 0
    for (int i = 0; i < graph.size(); i++) { result.push_back(std::pair(i, 0)); }

    /// Присваиваем вершинам из минимального доминирующего множества один и тот же цвет - 1
    std::pair<int, int> tempVertex;
    for (int i = 0; i < greedyDominateSet.size(); i++)
    {
        tempVertex = std::pair(greedyDominateSet[i], 1);
        result[greedyDominateSet[i]] = tempVertex;
    }

    /// Присваиваем оставшимся вершинам цвета в соответствии с условиями правильной раскраски
    for (int i = 0; i < result.size(); i++)
        if (result[i].second == 0)
            ColorizeVertex(graph, result, i);

    return result;
}

std::vector<std::pair<int, int>> BruteForceGraphColorising(std::vector<std::vector<int>> graph,
                                                           const std::function<unsigned int()>& rand = [](){return std::rand();})
{
    /// 0 - отсутствие цвета; другие числа - цвет
    std::vector<std::pair<int, int>> result;
    int sizeGraph = graph.size();

    /// Присваиваем всем вершинам цвет 0
    for (int i = 0; i < sizeGraph; i++) { result.push_back(std::pair(i, 0)); }

    /// Генерируем случайную стратовую вершину и присваиваем ей начальный цвет
    int indexStartVertex = rand() % result.size();
    std::pair<int, int> startVertex = std::pair(indexStartVertex, 1);
    result[indexStartVertex] = startVertex;

    for (int i = 0; i < result.size(); i++)
        if (i != indexStartVertex)
            ColorizeVertex(graph, result, i);

    return result;
}

std::vector<std::pair<int, int>> WelchPowelGraphColorising(View2_Linked& graph)
{
    /// 0 - отсутствие цвета; другие числа - цвет
    std::vector<std::pair<int, int>> result;
    /// Список пар (степень, вершина), необходим для сортировки по убыванию степеней
    std::vector<std::pair<int, int>> degreeVertices;

    /// Присваиваем всем вершинам цвет 0
    for (int i = 0; i < graph.size(); i++) { result.push_back(std::pair(i, 0)); }
    for (int i = 0; i < graph.size(); i++)
    {
        /// Считаем степени вершин
        std::pair<int, int> tempVertex = std::pair(0, i);
        for (int j = 0; j < graph.size(); j++)
            if (graph[i][j] == 1)
                tempVertex.first++;
        degreeVertices.push_back(tempVertex);
    }

    /// Сортируем степени по убыванию
    std::sort(degreeVertices.begin(), degreeVertices.end(),
              [] (auto& a, auto& b){return a.first > b.first; });

    std::pair<int, int> startVertex = std::pair(degreeVertices[0].second, 1);
    result[degreeVertices[0].second] = startVertex;
    for (int i = 1; i < graph.size(); i++)
        ColorizeVertex(graph, result, degreeVertices[i].second);

return result;
}


////////////////////////////////////////////////////////////////////////////
///////////////////Алгоритмы 9 лабораторной работы /////////////////////////
////////////////////////////////////////////////////////////////////////////



#endif //GRAPH_ALGORITHMS_H
