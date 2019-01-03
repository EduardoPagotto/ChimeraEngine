#ifndef __BSPTREE_ARRAY_TRIANGLE__HPP
#define __BSPTREE_ARRAY_TRIANGLE__HPP

#include "Triangle.hpp"

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