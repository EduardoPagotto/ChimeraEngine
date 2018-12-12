#ifndef __BSPTREE_LIST__HPP
#define __BSPTREE_LIST__HPP

#include "Polygon.hpp"

#include <vector>

class List {
  public:
    List();
    void Add(Polygon* plane);
    Polygon* Next();
    void resetNext() { nextindex = 0; }
    size_t size() { return lplanes.size(); }

  private:
    int nextindex;
    std::vector<Polygon*> lplanes;
};

#endif