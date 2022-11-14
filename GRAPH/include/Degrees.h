//
// Created by Азим on 18.09.2022.
//

#ifndef LAB1_DEGREES_H
#define LAB1_DEGREES_H

#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <utility>
#include <chrono>
#include <sstream>
#include <fstream>
#include <map>
#include <list>

using column = std::vector<int>;

struct Degrees : std::map<int,int>
{
    friend std::ostream& operator <<(std::ostream& stream,const Degrees& degrees)
    {
        for(auto& item : degrees)
            stream <<"(" << item.first << ") = " << item.second << std::endl;
        return stream;
    }
};


#endif //LAB1_DEGREES_H
