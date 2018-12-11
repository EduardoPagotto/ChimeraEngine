
#ifndef __POLYGON__HPP
#define __POLYGON__HPP

//#include <glm/glm.hpp>

class Vertex {
  public:
    Vertex();
    Vertex(float x, float y, float z);
    ~Vertex();
    float* getPoint();
    void setPoint(float x, float y, float z);

    float x;
    float y;
    float z;
};

class Polygon {
  public:
    Polygon();
    ~Polygon();
    Vertex* getVertices();
    void setVertices(Vertex p1, Vertex p2, Vertex p3);
    Vertex getNormals();
    void setNormals(Vertex n);
    void setColor(Vertex col);
    Vertex getColor();

    int id;
    Vertex vertices[3];
    Vertex normal;
    Vertex color;
};

#endif