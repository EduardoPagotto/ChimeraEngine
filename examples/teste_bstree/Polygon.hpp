
#ifndef __POLYGON__HPP
#define __POLYGON__HPP

#include <glm/glm.hpp>

class Polygon {
  public:
    Polygon();
    ~Polygon();
    glm::vec3* getVertices();
    void setVertices(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    glm::vec3 getNormals();
    void setNormals(glm::vec3 n);
    void setColor(glm::vec3 col);
    glm::vec3 getColor();

    int id;
    glm::vec3 vertices[3];
    glm::vec3 normal;
    glm::vec3 color;
};

#endif