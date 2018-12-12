#ifndef __BSPTREE_LIST_POLYGON__HPP
#define __BSPTREE_LIST_POLYGON__HPP

#include "Polygon.hpp"

#include <vector>

class ListPolygon {
  public:
    ListPolygon();
    void Add(Polygon* plane);
    Polygon* Next();
    void resetNext() { nextindex = 0; }
    size_t size() { return lplanes.size(); }

  private:
    int nextindex;
    std::vector<Polygon*> lplanes;
};

#endif