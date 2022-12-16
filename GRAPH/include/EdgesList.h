//
// Created by Азим on 18.09.2022.
//

#ifndef LAB1_EDGESLIST_H
#define LAB1_EDGESLIST_H

#include "Degrees.h"
#include "View4.h"

struct Edge
{
    Edge(int f, int t, int w):from(f), to(t), weight(w){}
    int from;
    int to;
    int weight;
};

//Список смежных вершин
struct EdgesList : public IView, public std::vector<Edge>
{
    EdgesList(const IView2& view);

    EdgesList();

    int countVertices;

    std::string to_string() const
    {
        std::stringstream stream;
        for(auto& item : (*this) )
            stream << item.from << "->" << item.to << "::" << item.weight << ";\n";
        return stream.str();
    }

    Degrees degree();
};


#endif //LAB1_EDGESLIST_H
