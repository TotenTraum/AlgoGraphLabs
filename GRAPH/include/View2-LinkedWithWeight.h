//
// Created by Азим on 20.09.2022.
//

#ifndef GRAPH_VIEW2W_LINKED_H
#define GRAPH_VIEW2W_LINKED_H

#include <functional>
#include "Degrees.h"
#include "IView.h"

//Матрица смежности (связный)
struct View2_LinkedW : public IView2
{
    View2_LinkedW(const int& n)
    {
        for(int i = 0; i < n; i++)
            emplace_back(n);
    }

    View2_LinkedW(View2_LinkedW& n)
    {
        for(int i = 0; i < n.size(); i++)
                emplace_back(n.size());

        for(int i = 0; i < n.size(); i++)
            for(int j = 0; j < n.size(); j++)
                at(i)[j] = n[i][j];
    }

    View2_LinkedW(const int& n, int k,
                  const std::function<unsigned int()>& rand = [](){return std::rand();});

    static int minWeight;
    static int maxWeight;
    Degrees degree();

    std::string to_string() const
    {
        std::stringstream stream;
        stream << ";;";
        for(int i=0; i < size(); i++)
            stream << i <<  ";";
        stream << std::endl;
        for(int i = 0; i < size(); i++)
        {
            stream << ";" << i << ";";
            for(auto& item : (*this)[i])
                stream << item << ";";
            stream << std::endl;
        }
        return stream.str();
    }

     void formMatrix(int& def, int& set, const int& max, int& k, const int& n, const std::function<unsigned int()>& rand);

    int getNumInRow(const int& row, const int& num, const int& startPos = 0);

    int getModule(const bool& repeating, const int& k, const int& row, const int& n);

    void fillRow(const int& row,const int& n, int n_edges,const std::function<unsigned int()>& rand);
};


#endif //GRAPH_VIEW2W_LINKED_H
