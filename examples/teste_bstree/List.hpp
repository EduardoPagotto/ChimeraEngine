#ifndef __BSPTREE_LIST__HPP
#define __BSPTREE_LIST__HPP

#include "Polygon.hpp"

class List {
  public:
    int nextindex;
    Polygon* planes;
    int noofplanes;
    List();
    void Add(Polygon* plane);
    Polygon* Next();
};

#endif