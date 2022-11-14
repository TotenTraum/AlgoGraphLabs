//
// Created by Азим on 18.09.2022.
//

#include "include/View3.h"

View3::View3(const View4& view)
{
    for(int i = 0;i < view.size(); i++)
        emplace_back();

    for(auto& item : view.links)
    {
        (*this)[item.first].push_back(item.second);
        (*this)[item.second].push_back(item.first);

    }
}

Degrees View3::degree()
{
    Degrees degrees;
    for(int i = 0 ; i < size(); i++)
        degrees[i] = (*this)[i].size() - 1;
    return degrees;
}