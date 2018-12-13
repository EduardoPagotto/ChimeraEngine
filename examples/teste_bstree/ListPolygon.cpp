
#include "ListPolygon.hpp"

ListPolygon::ListPolygon() { iIndex = lplanes.begin(); }

ListPolygon::ListPolygon(const ListPolygon& _cpy) {

    for (std::list<Polygon*>::iterator i = lplanes.begin(); i != lplanes.end(); i++) {
        Polygon* plane = (*i);
        this->lplanes.push_back(new Polygon(*plane));
    }

    iIndex = lplanes.begin();
}

Polygon* ListPolygon::getFromList() {

    if (lplanes.empty() == false) {
        Polygon* p = lplanes.front();
        lplanes.pop_front();
        return p;
    }

    return nullptr;
}

Polygon* ListPolygon::next() {

    if (iIndex != lplanes.end()) {
        Polygon* p = (*iIndex);
        iIndex++;
        return p;
    }
    return nullptr;
}
