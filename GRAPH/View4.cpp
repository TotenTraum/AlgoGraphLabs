//
// Created by Азим on 18.09.2022.
//

#include "include/View4.h"

View4::View4(const IView2& matrix)
{
    std::set<std::pair<int,int>> set;
    for(int i=0; i < matrix.size(); i++)
        for(int j = 0; j < matrix.size(); j++)
        {
            if(matrix[i][j] != 0 )
            {
                int a = i, b = j;
                if (a > b) {
                    std::swap(a, b);
                }
                set.emplace(a, b);
            }
        }
    for(auto& item : set)
        links.push_back(item);

    for(int i = 0; i < matrix.size();i++)
        push_back(column(links.size()));

    for(int i = 0; i < matrix.size(); i++)
    {
        int j = 0;
        for(auto& item : links)
        {
            if(i == item.first || i == item.second )
                (*this)[i][j] = 1;
            j++;
        }
    }
}

Degrees View4::degree()
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
