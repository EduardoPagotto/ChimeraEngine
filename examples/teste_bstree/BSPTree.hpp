#ifndef __BSTREE__HPP
#define __BSTREE__HPP

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

#include "Polygon.hpp"

class Octahedron {
  public:
    Octahedron();
    ~Octahedron();
    Vertex vertices[6];
    Polygon faces[8];
    Vertex* getVertices();
    void setVertices(Vertex* verts);
    Polygon* getFaces();
    void setFaces(Polygon* faces);
};

class Cube {
  public:
    Cube();
    ~Cube();
    Polygon faces[10];
    Vertex normals[10];
    Vertex* getNormal();
    Vertex colors[10];
    void setNormal(Vertex* normal);
    Polygon* getFaces();
    void setFaces(Polygon* face);
    Vertex* getColors();
    void setColors(Vertex* cols);
};

#endif