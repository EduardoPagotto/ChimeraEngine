#ifndef __BSPTREE_LIST_POLYGON__HPP
#define __BSPTREE_LIST_POLYGON__HPP

#include "Polygon.hpp"

#include <list>

class ArrayTriangle {
  public:
    ArrayTriangle();
    ArrayTriangle(const ArrayTriangle& _cpy);
    ~ArrayTriangle();

    inline void addToList(Triangle* plane) { lplanes.push_back(new Triangle(*plane)); }
    inline bool isEmpty() { return lplanes.empty(); }
    inline size_t size() { return lplanes.size(); }
    inline void begin() { iIndex = lplanes.begin(); }

    // void rotate(const int& indice);

    Triangle* getFromList();
    Triangle* next();

  private:
    std::list<Triangle*>::iterator iIndex;
    std::list<Triangle*> lplanes;
};

#endif