//
// Created by Азим on 18.09.2022.
//

#include "include/EdgesList.h"

EdgesList::EdgesList(const IView2 &view) {
    countVertices = view.size();
    for (int i = 0; i < view.size(); ++i)
        for (int j = i; j < view.size(); ++j)
            if(view[i][j] != 0)
                (*this).emplace_back(Edge(i, j, view[i][j]));
}

EdgesList::EdgesList() {}

Degrees EdgesList::degree()
{
    Degrees degrees;
    for (auto item: (*this))
        degrees[item.from]++;
    return degrees;
}