#include <functional>
#include <memory>
#include "View2.h"
#include "View4.h"
#include "View2-Linked.h"
#include "algorithms.h"
#include "LinkedDirectedGraph.h"
#include <list>
#include <set>
#include <stack>

std::string src_dir = "lab6-";

void gen_Vn_Ek(int& n, int& k, const std::function<unsigned int()>& func)
{
    n = 20 + (func() % 11);
    n = 5;
    int max = n * (n-1) /2;
    do
        k =  func() % (max + 1);
    while (k < n - 1);
}

void printSet(std::ostream& stream, const std::set<int>& mtx)
{
    stream << ";";
    for(auto& item : mtx)
        stream << item << ";";
}

void printList(std::ostream& stream, const std::list<std::set<int>>& mtx)
{
    for(auto& item : mtx)
    {
        printSet(stream,item);
        stream << std::endl;
    }
}

size_t Max(std::list<std::set<int>>& sets)
{
    size_t max = 0;
    for(auto& item : sets)
        max = (max < item.size()) ? item.size() : max;
    return max;
}

std::set<int> getMaxSet(std::list<std::set<int>>& sets)
{

    size_t max = Max(sets);
    std::set<int> val;
    for(auto& item : sets)
        if(item.size() == max)
        val = item;
    return val;
}

void tasks(std::ofstream& file,View2_Linked& matrix)
{
    auto resultKerbosh = kerbosh(matrix);
    auto resultGreedy = greedyDominateSetAlgorithm(matrix);

    /// Вывод матрицы
    file << "Матрица смежности;\n";
    file << matrix.to_string();
    /// Вывод результата алгоритма Брона-Кербоша
    file << "Максимальное независимое множество ;\n";
    printSet(file, getMaxSet(resultKerbosh));
    file << "\nчисло независимости графа;";
    file << Max(resultKerbosh)  << ";\n";
    file << "минимальное доминирующее множество ;\n";
    printSet(file,resultGreedy);
    file << std::endl;
    file << "число доминирования графа;";
    file << resultGreedy.size() << ";\n";
    file.close();
}

int main() {
    std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
    int n,k;
    std::ofstream file;

    gen_Vn_Ek(n,k, [&generator](){return generator();});

    View2_Linked matrix =  View2_Linked(n,k, generator);
    file.open(src_dir + "LinkDirGraph.csv");
    tasks(file, matrix);
    file << std::endl;
    file << std::endl;
    file.close();

    auto matrixInvert = new View2_Linked(matrix);
    Invert(*matrixInvert);
    file.open(src_dir + "LinkDirGraph.csv", std::ios::app);
    tasks(file, *matrixInvert);
    file.close();

    return 0;
}
