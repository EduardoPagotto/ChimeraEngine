
#ifndef __POLYGON__HPP
#define __POLYGON__HPP

#include <glm/glm.hpp>

class Polygon {
  public:
    Polygon();
    Polygon(const int& _id);
    Polygon(const Polygon& _cpy);

    void computeFaceNormalsFromVertices();

    inline void setId(const int& _id) { this->id = _id; }
    inline void setColor(const glm::vec3& _color) { this->color = _color; }

    void setVertices(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    void setNormais(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

    inline int getId() const { return this->id; }
    inline glm::vec3 getColor() const { return this->color; }
    inline glm::vec3* getVertices() { return vertices; }
    inline glm::vec3* getNormais() { return vertices; }
    inline glm::vec3 getFaceNormal() const { return this->faceNormal; }

  private:
    void computeFaceNormalFromNormais();

    int id;
    glm::vec3 faceNormal;
    glm::vec3 color;
    glm::vec3 vertices[3];
    glm::vec3 normais[3];
};
#endif
