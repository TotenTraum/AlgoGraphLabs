//
// Created by Азим on 18.09.2022.
//

#include "include/View2.h"

View2::View2(const int &n, int k) {
    int def = 0, set = 1;
    int max = n*(n-1)/2;
    std::stringstream stream;
    auto start = std::chrono::system_clock::now();
    stream << "\"n=\";" << n << ";" << "\"k=\";" << k << "; max;" << max << ";"<< std::endl;

    //if(k > max/2)
    //{
    //    k = max - k;
    //    def = 1;
    //    set = 0;
    //}

    for(int i=0; i < n; i++)
        this->push_back(column(n,def));

    if(def == 0)
        for(int i = 0;i< n;i++)
            (*this)[i][i] = 0;

    bool again = false;
    int counter = 0;

    for(int i=0; k; i++)
    {
        if(i == n)
        {
            i = 0;
            again = true;
        }
        int module = 0;
        if(again)
        {
            for(int j = i; j < n; j++)
                module +=  ((*this)[i][j] != def) ? set : def;
            module = (n - i - module);
            if(module > k)
                module = k + 1;
        }
        else
            module = ((n-i - 1) > k) ? k + 1 : n-i - 1;

        if(module <= 0)
            continue;

        int n_edges = std::rand() % module;
        k-= n_edges;

        for(int j = i + 1; n_edges; j++)
        {

            if(j == n) // Достиг конец таблицы
                j = i;

            if(i == j) // Петля
                continue;

            if((*this)[i][j] == set) // Уже есть связь
                continue;

            if(std::rand() % 2 == 0)
            {
                (*this)[i][j] = (*this)[j][i] = set;
                n_edges--;
            }
        }
        auto tm = std::chrono::system_clock::now() - start;
        stream << to_string() << std::endl << "Iteration;" << ++counter << ";K;" << k <<"; ms =;"
               << duration_cast<std::chrono::milliseconds >(tm).count() << ";" << std::endl << std::endl;
    }
    std::ofstream file;
    file.open("generation-steps.csv");
    file << stream.rdbuf();
    file.close();
}

Degrees View2::degree()
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