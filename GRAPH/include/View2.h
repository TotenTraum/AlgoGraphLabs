//
// Created by Азим on 18.09.2022.
//

#ifndef LAB1_VIEW2_H
#define LAB1_VIEW2_H

#include "IView.h"

//Матрица смежности
struct View2 : public IView2
{
    View2(const int& n, int k);

    View2(const int& n);
    Degrees degree();

    std::string to_string() const
    {
        std::stringstream stream;
        stream << ";;";
        for(int i=0;i<size();i++)
            stream << i <<  ";";
        stream << std::endl;
        for(int i = 0; i < size(); i++)
        {
            stream <<";" << i << ";";
            for(auto& item : (*this)[i])
                stream << item << ";";
            stream << std::endl;
        }
        return stream.str();
    }
};

#endif //LAB1_VIEW2_H
