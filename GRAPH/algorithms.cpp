////
//// Created by Азим on 20.09.2022.
////
//
//#include "algorithms.h"
//
//std::tuple<std::vector<int>, std::vector<int>> BellmanFord(IView2& mtx, int src)
//{
//    const int inf = std::numeric_limits<int>::max();
//    /// Заполнение массива индексов
//    std::vector<int> next(mtx.size(), inf);
//
//    /// Заполнение расстояний
//    std::vector<int> dist(mtx.size(), inf);
//    dist[src] = 0;
//    /// Алгоритм поиска пути
//    for (int k = 0; k < mtx.size(); ++k)
//        for (int i = 0; i < mtx.size(); ++i)
//            for (int j = 0; j < mtx.size(); ++j)
//            {
//                long long w = mtx[i][j];
//                if(w != 0)
//                    if (dist[i] != inf and dist[i] + w < dist[j]) {
//                        dist[j] = dist[i] + w;
//                        next[j] = i;
//                    }
//            }
//    return std::make_tuple(dist,next);
//}
//
//std::vector<int> RestoreBellmanFord(std::vector<int> mtx, int from, int to)
//{
//    const int inf = std::numeric_limits<int>::max();
//    auto path = std::vector<int> {};
//    for (auto current = to; current != from && current != inf; current = mtx[current])
//        path.push_back(current);
//    path.push_back(from);
//    std::reverse(path.begin(), path.end());
//    return path;
//}
//
//std::tuple<Matrix, Matrix> Floyd(IView2& mtx)
//{
//    const int inf = std::numeric_limits<int>::max();
//    /// Заполнение массива индексов
//    Matrix next(mtx.size(), std::vector<int>(mtx.size()));
//    for (auto i = 0; i < mtx.size(); ++i)
//        for (auto j = 0; j < mtx.size(); ++j)
//            next[i][j] = j;
//    /// Заполнение расстояний
//    Matrix dist = mtx;
//    for (int i = 0; i < mtx.size(); ++i)
//        for (int j = i + 1; j < mtx.size(); ++j)
//            if (dist[i][j] == 0)
//                dist[i][j] = dist[j][i] = inf;
//    /// Алгоритм поиска пути
//    for (int k = 0; k < mtx.size(); ++k)
//        for (int i = 0; i < mtx.size(); ++i)
//            for (int j = 0; j < mtx.size(); ++j)
//            {
//                long long sum = (long long) dist[i][k] + (long long)dist[k][j];
//                if (sum < (long long)dist[i][j]) {
//                    dist[i][j] = sum;
//                    next[i][j] = next[i][k];
//                }
//            }
//    return std::make_tuple(dist,next);
//}
//
//std::vector<int> RestoreFloyd(Matrix mtx, int from, int to)
//{
//    auto path = std::vector<int> {from};
//    while (from != to)
//    {
//        from = mtx[from][to];
//        path.push_back(from);
//    }
//    return path;
//}
//
//int FindNearest(std::vector<int> distance, std::vector<bool> visited)
//{
//    static int inf = std::numeric_limits<int>::max();
//    int minDistance = inf;
//    int minIndex = 0;
//
//    for (int i = 0; i < distance.size(); ++i)
//        if (!visited[i] && distance[i] <= minDistance)
//        {
//            minDistance = distance[i];
//            minIndex = i;
//        }
//    return minIndex;
//}
//
//std::tuple<std::vector<int>, std::vector<int>> Dikstra(IView2& mtx, int src)
//{
//    const int inf = std::numeric_limits<int>::max();
//    auto visited = std::vector<bool>(mtx.size());
//    auto distance = std::vector<int>(mtx.size(), inf);
//    auto buildingPaths = std::vector<int>(mtx.size(), inf);
//    distance[src] = 0;
//
//    for (int i = 0; i < mtx.size() - 1; ++i)
//    {
//        int current = FindNearest(distance, visited);
//        visited[current] = true;
//        for (int j = 0; j < mtx.size(); ++j)
//            if (!visited[j] && mtx[current][j] != 0 && distance[current] != inf && distance[current] + mtx[current][j] < distance[j])
//            {
//                distance[j] = distance[current] + mtx[current][j];
//                buildingPaths[j] = current;
//            }
//    }
//
//    return std::make_tuple(distance, buildingPaths);
//}
//
//std::tuple<EdgesList,int> BuildPrim(EdgesList graph)
//{
//    EdgesList mst{};
//    std::vector<bool> visited(graph.countVertices);
//    auto unused = EdgesList(graph);
//    visited[0] = true;
//
//    auto weight = 0;
//    while (std::any_of(visited.begin(), visited.end(),[](auto a){return a == false;}))
//    {
//        int minEdgeIndex = FindMinEdgeFromUnused(unused, visited);
//        auto minEdge = unused[minEdgeIndex];
//
//        if (visited[minEdge.from]) { visited[minEdge.to] = true; }
//        else { visited[minEdge.from] = true; }
//
//        mst.emplace_back(minEdge.from, minEdge.to, minEdge.weight);
//        unused.erase(std::next(unused.begin(),minEdgeIndex));
//        weight += minEdge.weight;
//    }
//
//    return std::make_tuple(mst, weight);
//}