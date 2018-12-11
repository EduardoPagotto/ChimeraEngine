#ifndef __BSTREE__HPP
#define __BSTREE__HPP

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

#include "Polygon.hpp"

enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

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

class List {
  public:
    int nextindex;
    Polygon* planes;
    int noofplanes;
    List();
    void Add(Polygon* plane);
    Polygon* Next();
};

class Node {
  public:
    Polygon partition;
    List polygons;
    Node* frontNode;
    Node* backNode;
    Node();
    void BuildTree(Node* rootNode, List polygons);
    SIDE classifyPolygon(Polygon* cPolygon);
    float classify(Vertex* plane, Vertex* eye);
    void DrawTree(Node* finaltree, Vertex* eyepoint, List* finalfaces);
};

#endif