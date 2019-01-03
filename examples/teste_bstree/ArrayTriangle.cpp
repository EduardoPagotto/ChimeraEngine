
#include "ListPolygon.hpp"
#include <algorithm>

ArrayTriangle::ArrayTriangle() { iIndex = lplanes.begin(); }

ArrayTriangle::~ArrayTriangle() {

    for (std::list<Triangle*>::iterator i = lplanes.begin(); i != lplanes.end(); i++) {
        Triangle* plane = (*i);
        delete plane;
        plane = nullptr;
    }

    lplanes.clear();
}

ArrayTriangle::ArrayTriangle(const ArrayTriangle& _cpy) {

    std::list<Triangle*> old = _cpy.lplanes;
    for (std::list<Triangle*>::iterator i = old.begin(); i != old.end(); i++) {
        Triangle* plane = (*i);
        this->lplanes.push_back(new Triangle(*plane));
    }

    iIndex = lplanes.begin();
}

Triangle* ArrayTriangle::getFromList() {

    if (lplanes.empty() == false) {
        Triangle* p = lplanes.front();
        lplanes.pop_front();
        return p;
    }

    return nullptr;
}

Triangle* ArrayTriangle::next() {

    if (iIndex != lplanes.end()) {
        Triangle* p = (*iIndex);
        iIndex++;
        return p;
    }
    return nullptr;
}

// void ArrayTriangle::rotate(const int& indice) { std::rotate(lplanes.begin(), lplanes.begin() + 2, lplanes.end()); }