#include <malloc.h>

#include "ListPolygon.hpp"

ListPolygon::ListPolygon() { nextindex = 0; }

void ListPolygon::Add(Polygon* plane) { lplanes.push_back(createCpyPolygon(plane)); }

Polygon* ListPolygon::Next() {

    Polygon* p = NULL;
    if (nextindex == lplanes.size())
        return p;

    p = createCpyPolygon(lplanes[nextindex]);

    nextindex++;
    return p;
}