#include "BSPTree.hpp"
#include <malloc.h>
#include <stdio.h>

bool v = true;

Cube::Cube() {}

Cube::~Cube() {}

Polygon* Cube::getFaces() { return Cube::faces; }

glm::vec3* Cube::getNormal() { return Cube::normals; }

void Cube::setFaces(Polygon* face) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            Cube::faces[i].vertices[j].x = face[i].vertices[j].x;
            Cube::faces[i].vertices[j].y = face[i].vertices[j].y;
            Cube::faces[i].vertices[j].z = face[i].vertices[j].z;
        }
    }
}

void Cube::setNormal(glm::vec3* normal) {
    for (int i = 0; i < 10; i++) {
        Cube::normals[i].x = normal[i].x;
        Cube::normals[i].y = normal[i].y;
        Cube::normals[i].z = normal[i].z;
    }
}

glm::vec3* Cube::getColors() { return Cube::colors; }

void Cube::setColors(glm::vec3* cols) {
    for (int i = 0; i < 10; i++)
        // Cube::colors[i].setPoint(cols[i].x, cols[i].y, cols[i].z);
        Cube::colors[i] = cols[i];
}

Octahedron::Octahedron() {}

Octahedron::~Octahedron() {}

Polygon* Octahedron::getFaces() { return Octahedron::faces; }

void Octahedron::setFaces(Polygon* inface) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            Octahedron::faces[i].vertices[j].x = inface[i].vertices[j].x;
            Octahedron::faces[i].vertices[j].y = inface[i].vertices[j].y;
            Octahedron::faces[i].vertices[j].z = inface[i].vertices[j].z;
        }
        Octahedron::faces[i].normal.x = inface[i].normal.x;
        Octahedron::faces[i].normal.y = inface[i].normal.y;
        Octahedron::faces[i].normal.z = inface[i].normal.z;
    }
}

glm::vec3* Octahedron::getVertices() { return Octahedron::vertices; }

void Octahedron::setVertices(glm::vec3* verts) {
    for (int i = 0; i < 6; i++) {
        Octahedron::vertices[i].x = verts[i].x;
        Octahedron::vertices[i].y = verts[i].y;
        Octahedron::vertices[i].z = verts[i].z;
    }
}
