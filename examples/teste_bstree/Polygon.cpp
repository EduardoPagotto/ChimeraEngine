#include "Polygon.hpp"

Vertex::Vertex() {}

Vertex::~Vertex() {
    Vertex::x = 0;
    Vertex::y = 0;
    Vertex::z = 0;
}

Vertex::Vertex(float x, float y, float z) {
    Vertex::x = x;
    Vertex::y = y;
    Vertex::z = z;
}

float* Vertex::getPoint() {
    float vertices[3];
    vertices[0] = Vertex::x;
    vertices[1] = Vertex::y;
    vertices[2] = Vertex::z;
    return vertices;
}

void Vertex::setPoint(float x, float y, float z) {
    Vertex::x = x;
    Vertex::y = y;
    Vertex::z = z;
}

Polygon::Polygon() {}

Polygon::~Polygon() {}

void Polygon::setNormals(Vertex n) { Polygon::normal = n; }

void Polygon::setVertices(Vertex p1, Vertex p2, Vertex p3) {
    Polygon::vertices[0].setPoint(p1.x, p1.y, p1.z);
    Polygon::vertices[1].setPoint(p2.x, p2.y, p2.z);
    Polygon::vertices[2].setPoint(p3.x, p3.y, p3.z);
}

Vertex Polygon::getColor() { return Polygon::color; }

void Polygon::setColor(Vertex col) {
    Polygon::color.x = col.x;
    Polygon::color.y = col.y;
    Polygon::color.z = col.z;
}

Vertex* Polygon::getVertices() { return Polygon::vertices; }

Vertex Polygon::getNormals() { return Polygon::normal; }