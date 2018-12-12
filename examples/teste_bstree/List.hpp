#ifndef __BSPTREE_LIST__HPP
#define __BSPTREE_LIST__HPP

#include "Polygon.hpp"

class List {
  public:
    List();
    void Add(Polygon* plane);
    Polygon* Next();

    int nextindex;
    int noofplanes;
    Polygon* planes;
};

#endif