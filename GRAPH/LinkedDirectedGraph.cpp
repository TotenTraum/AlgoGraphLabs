//
// Created by Азим on 25.10.2022.
//

#include "LinkedDirectedGraph.h"

int LinkDirGraph::getNumInRow(const int& row,const int& num, const int& startPos)
{
    int nums = 0;
    for(int j = startPos; j < at(row).size(); j++)
        nums += ((*this)[row][j] == num) ? 1 : 0;
    return nums;
}

int LinkDirGraph::getModule(const bool& repeating,const int& k, const int& row, const int& n)
{
    int module;

    module = (n - getNumInRow(row,1));
    if(module > k)
        module = k + 1;

    return module;
}

LinkDirGraph::LinkDirGraph(const int &n, int k, const std::function<unsigned int()>& Random) {
    int max = n*(n-1)/2;
    int tmpK = k;

    formMatrix(max,k,n,Random);

    bool again = false;

    for(int i=0; k>0; i++)
    {
        if(i == n)
        {
            i = 0;
            again = true;
        }
        int module = getModule(again,k,i,n);
        if(module <= 0)
            continue;

        int n_edges = Random() % module;
        k-= n_edges;

        fillRow(i, n, n_edges,Random);
    }
}

Degrees LinkDirGraph::degree()
{
    Degrees degrees;
    for(int i = 0; i < size();i++)
    {
        int sum = 0;
        for(auto& item : (*this)[i])
            sum += item;
        degrees[i] = sum;
    }
    return degrees;
}

void LinkDirGraph::fillRow(const int &row,const int& n,int n_edges, const std::function<unsigned int()>& Random)
{
    for(int j = 0; n_edges != 0 ; j++)
    {
        if(j == n) // Достиг конец таблицы
            j = 0;

        if(row == j) // Петля
            continue;

        if((*this)[row][j] == 1 || (*this)[row][j] == -1 ) // Уже есть связь
            continue;

        if(Random() % (n - 1) == 0)
        {
            (*this)[row][j] = 1;
            n_edges--;
        }
    }
}

void LinkDirGraph::formMatrix(const int &max, int &k, const int &n,const std::function<unsigned int()>& Rand)
{
    for(int i=0; i < n; i++)
        this->push_back(column(n,0));

    k -= (n);
    for(int i = 0 ; i < n; i++)
        fillRow(i,n,1,Rand);
}
