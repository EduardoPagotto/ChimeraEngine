#ifndef __BSPTREE_LIST_POLYGON__HPP
#define __BSPTREE_LIST_POLYGON__HPP

#include "Polygon.hpp"

#include <list>

class ListPolygon {
  public:
    ListPolygon();
    ListPolygon(const ListPolygon& _cpy);
    ~ListPolygon();

    inline void addToList(Polygon* plane) { lplanes.push_back(new Polygon(*plane)); }
    inline bool isEmpty() { return lplanes.empty(); }
    inline size_t size() { return lplanes.size(); }
    inline void begin() { iIndex = lplanes.begin(); }

    Polygon* getFromList();
    Polygon* next();

  private:
    std::list<Polygon*>::iterator iIndex;
    std::list<Polygon*> lplanes;
};

#endif