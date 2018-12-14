
#ifndef __POLYGON__HPP
#define __POLYGON__HPP

#include <glm/glm.hpp>

class Polygon {
  public:
    Polygon();
    Polygon(const int& _id);
    Polygon(const Polygon& _cpy);

    void computeNormalVertices();

    void setVertices(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    inline void setNormal(const glm::vec3& _normal) { this->normal = _normal; }
    inline void setColor(const glm::vec3& _color) { this->color = _color; }
    inline void setId(const int& _id) { this->id = _id; }

    inline int getId() const { return this->id; }
    inline glm::vec3 getNormal() const { return this->normal; }
    inline glm::vec3 getColor() const { return this->color; }
    inline glm::vec3* getVertices() { return vertices; }

  private:
    int id;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec3 vertices[3];
};
#endif
