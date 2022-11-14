//
// Created by Азим on 20.09.2022.
//

#ifndef GRAPH_IVIEW_H
#define GRAPH_IVIEW_H

#include "Degrees.h"

class IView {
public:
    virtual Degrees degree() = 0;

    virtual std::string to_string() const = 0;
};

class IView2 : public IView, public std::vector<column> {};


#endif //GRAPH_IVIEW_H
