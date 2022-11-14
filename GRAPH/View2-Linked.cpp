//
// Created by Азим on 20.09.2022.
//

#include "include/View2-Linked.h"

int View2_Linked::getNumInRow(const int& row,const int& num, const int& startPos)
{
    int nums = 0;
    for(int j = startPos; j < at(row).size(); j++)
        nums += ((*this)[row][j] == num) ? 1 : 0;
    return nums;
}

int View2_Linked::getModule(const bool& repeating,const int& k, const int& row, const int& n)
{
    int module;
    if(repeating)
    {
        module = (n - row - getNumInRow(row,1,row));
        if(module > k)
            module = k + 1;
    }
    else
        module = ((n- row - 1) > k) ? k + 1 : n - row - 1;
    return module;
}

View2_Linked::View2_Linked(const int &n, int k,const std::function<unsigned int()>& Random) {
    int def = 0, set = 1;
    int max = n*(n-1)/2;
    std::stringstream stream;
    auto start = std::chrono::system_clock::now();
    stream << "\"n=\";" << n << ";" << "\"k=\";" << k << "; max;" << max << ";"<< std::endl;

    formMatrix(def,set,max,k,n,Random);

    bool again = false;
    int counter = 0;

    for(int i=0; k; i++)
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

        fillRow(i,n,set,n_edges,Random);

        auto tm = std::chrono::system_clock::now() - start;
        stream << to_string() << std::endl << "Iteration;" << ++counter << ";K;" << k <<"; ms =;"
               << duration_cast<std::chrono::milliseconds >(tm).count() << ";" << std::endl << std::endl;
    }

    std::ofstream file;
    file.open("generation-steps.csv");
    file << stream.rdbuf();
    file.close();
}

Degrees View2_Linked::degree()
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

void View2_Linked::fillRow(const int &row,const int& n,const int& set, int n_edges, const std::function<unsigned int()>& Random)
{
    for(int j = row + 1; n_edges != 0 ; j++)
    {
        if(j == n) // Достиг конец таблицы
            j = row;

        if(row == j) // Петля
            continue;

        if((*this)[row][j] == set) // Уже есть связь
            continue;

        if(Random() % 2 == 0)
        {
            (*this)[row][j] = (*this)[j][row] = set;
            n_edges--;
        }
    }
}

void View2_Linked::formMatrix(int &def, int &set, const int &max, int &k, const int &n,const std::function<unsigned int()>& Rand) {

    for(int i=0; i < n; i++)
        this->push_back(column(n,def));

    if(def != 0)
        for(int i = 0;i< n;i++)
            (*this)[i][i] = 0;

    k -= (n - 1);
    for(int i = 0 ; i < n - 1; i++)
        fillRow(i,n,set,1,Rand);
}
