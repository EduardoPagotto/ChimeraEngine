
#include "ListPolygon.hpp"
#include <algorithm>

ListPolygon::ListPolygon() { iIndex = lplanes.begin(); }

ListPolygon::~ListPolygon() {

    for (std::list<Polygon*>::iterator i = lplanes.begin(); i != lplanes.end(); i++) {
        Polygon* plane = (*i);
        delete plane;
        plane = nullptr;
    }

    lplanes.clear();
}

ListPolygon::ListPolygon(const ListPolygon& _cpy) {

    std::list<Polygon*> old = _cpy.lplanes;
    for (std::list<Polygon*>::iterator i = old.begin(); i != old.end(); i++) {
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

// void ListPolygon::rotate(const int& indice) { std::rotate(lplanes.begin(), lplanes.begin() + 2, lplanes.end()); }