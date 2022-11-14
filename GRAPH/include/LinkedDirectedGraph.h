//
// Created by Азим on 25.10.2022.
//

#ifndef GENERATOR_LINKEDDIRECTEDGRAPH_H
#define GENERATOR_LINKEDDIRECTEDGRAPH_H

#include <functional>
#include "Degrees.h"
#include "IView.h"

//Матрица смежности (связный)
struct LinkDirGraph : public IView2
{
    LinkDirGraph(const int& n, int k,const std::function<unsigned int()>& rand = [](){return std::rand();});

    Degrees degree();

    std::string to_string() const
    {
        std::stringstream stream;
        stream << ";";
        for(int i=0;i <size();i++)
            stream << i <<  ";";
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

    void formMatrix(const int& max, int& k, const int& n, const std::function<unsigned int()>& rand);

    int getNumInRow(const int& row, const int& num, const int& startPos = 0);

    int getModule(const bool& repeating, const int& k, const int& row, const int& n);

    void fillRow(const int& row,const int& n,int n_edges,const std::function<unsigned int()>& rand);
};


#endif //GENERATOR_LINKEDDIRECTEDGRAPH_H
