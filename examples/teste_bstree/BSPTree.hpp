#ifndef __BSTREE__HPP
#define __BSTREE__HPP

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

#include "Polygon.hpp"

class Octahedron {
  public:
    Octahedron();
    ~Octahedron();
    glm::vec3 vertices[6];
    Polygon faces[8];
    glm::vec3* getVertices();
    void setVertices(glm::vec3* verts);
    Polygon* getFaces();
    void setFaces(Polygon* faces);
};

class Cube {
  public:
    Cube();
    ~Cube();
    Polygon faces[10];
    glm::vec3 normals[10];
    glm::vec3* getNormal();
    glm::vec3 colors[10];
    void setNormal(glm::vec3* normal);
    Polygon* getFaces();
    void setFaces(Polygon* face);
    glm::vec3* getColors();
    void setColors(glm::vec3* cols);
};

#endif