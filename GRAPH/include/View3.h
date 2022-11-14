//
// Created by Азим on 18.09.2022.
//

#ifndef LAB1_VIEW3_H
#define LAB1_VIEW3_H

#include "Degrees.h"
#include "View4.h"

//Список смежных вершин
struct View3 : public IView,std::vector<std::list<int>>
{
View3(const View4& view);

std::string to_string() const
{
    std::stringstream stream;
    for(int i=0; i < size();i++)
    {
        stream << i << ": ";
        for(auto& item : (*this)[i] )
            stream << item << ",";
        stream << std::endl;
    }
    return stream.str();
}

    Degrees degree();
};


#endif //LAB1_VIEW3_H
