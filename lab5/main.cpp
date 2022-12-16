#include <functional>
#include <memory>
#include "View2.h"
#include "View2-LinkedWithWeight.h"
#include "algorithms.h"
#include <list>
#include <set>
#include <stack>

std::string src_dir = "lab5-";

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

void printMtxToList(std::ostream& stream, IView2& mtx)
{
    for(auto i = 0; i < mtx.size();i++)
    {
        stream << i << ";";
        for (int j = 0; j < mtx.size(); ++j) {
            if(mtx[i][j] != 0)
                stream << "(" << j << "/" << mtx[i][j] << ");";
        }
        stream << std::endl;
    }
}

size_t max(std::list<std::set<int>>& sets)
{
    size_t max = 0;
    for(auto& item : sets)
        max = (max < item.size()) ? item.size() : max;
    return max;
}

void tasks(std::ofstream& file,IView2& matrix)
{
    /// Вывод матрицы
    file << "Матрица смежности;\n";
    file << matrix.to_string();
    {
        file << "\nКрускал для матрицы смежности;\n";
        auto [mtx, W] = BuildKruskal(matrix);
        file << "Вес;" << W << ";\n";
        file << "Матрица;\n" << mtx->to_string() << ";";
    }
    {
        file << "\nПрим для матрицы смежности;\n";
        auto [mtx, W] = BuildPrim(matrix);
        file << "Вес;" << W << ";\n";
        file << "Матрица;\n" << mtx->to_string() << ";\n";
    }
    EdgesList list(matrix);
    /// Вывод списка
    file << "Список ребер;\n";
    file << list.to_string();
    {
        file << "\nКрускал для списка ребер;\n";
        auto [mtx, W] = BuildKruskal(list);
        file << "Вес;" << W << ";\n";
        file << "Список;\n" << mtx.to_string() << ";";
    }
    {
        file << "\nПрим для списка ребер;\n";
        auto [mtx, W] = BuildPrim(list);
        file << "Вес;" << W << ";\n";
        file << "Список;\n" << mtx.to_string() << ";";
    }
}

int main() {
    std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
    int n,k;
    std::ofstream file;

    gen_Vn_Ek(n,k, [&generator](){return generator();});

    IView2* matrix = new View2_LinkedW(n, k, generator);

    file.open(src_dir + "LinkDirGraph.csv");
    tasks(file, *matrix);
    file.close();

    return 0;
}
