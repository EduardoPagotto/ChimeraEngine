#include <malloc.h>

#include "List.hpp"

List::List() { List::nextindex = 0; }

void List::Add(Polygon* plane) { lplanes.push_back(createCpyPolygon(plane)); }

Polygon* List::Next() {

    Polygon* p = NULL;
    if (nextindex == lplanes.size())
        return p;

    p = createCpyPolygon(lplanes[nextindex]);

    nextindex++;
    return p;
}