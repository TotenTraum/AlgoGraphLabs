//
// Created by Азим on 18.09.2022.
//

#ifndef LAB1_VIEW4_H
#define LAB1_VIEW4_H

#include "Degrees.h"
#include "View2.h"

//Матрица инцидентности
struct View4 :public IView, std::vector<column>
{
    std::vector<std::pair<int,int>> links;

    View4(const IView2& matrix);

    Degrees degree();

    std::string to_string() const
    {
        std::stringstream stream;
        stream << ";";
        for(auto& item : links)
            stream << "\"(" << item.first << ";" << item.second << ")\";";
        stream << std::endl;
        for(int i = 0; i < size(); i++)
        {
            stream << i << ";";
            for(auto& item : (*this)[i])
                stream << item << ";";
            stream << std::endl;
        }
        return stream.str();
    }
};


#endif //LAB1_VIEW4_H
