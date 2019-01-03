
#include "ListPolygon.hpp"
#include <algorithm>

ListPolygon::ListPolygon() { iIndex = lplanes.begin(); }

ListPolygon::~ListPolygon() {

    for (std::list<Triangle*>::iterator i = lplanes.begin(); i != lplanes.end(); i++) {
        Triangle* plane = (*i);
        delete plane;
        plane = nullptr;
    }

    lplanes.clear();
}

ListPolygon::ListPolygon(const ListPolygon& _cpy) {

    std::list<Triangle*> old = _cpy.lplanes;
    for (std::list<Triangle*>::iterator i = old.begin(); i != old.end(); i++) {
        Triangle* plane = (*i);
        this->lplanes.push_back(new Triangle(*plane));
    }

    iIndex = lplanes.begin();
}

Triangle* ListPolygon::getFromList() {

    if (lplanes.empty() == false) {
        Triangle* p = lplanes.front();
        lplanes.pop_front();
        return p;
    }

    return nullptr;
}

Triangle* ListPolygon::next() {

    if (iIndex != lplanes.end()) {
        Triangle* p = (*iIndex);
        iIndex++;
        return p;
    }
    return nullptr;
}

// void ListPolygon::rotate(const int& indice) { std::rotate(lplanes.begin(), lplanes.begin() + 2, lplanes.end()); }