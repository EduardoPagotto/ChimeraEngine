
#ifndef __TRIANGLE__HPP
#define __TRIANGLE__HPP

#include <glm/glm.hpp>

class Triangle {
  public:
    Triangle();
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    void computeFaceNormalsFromVertices();
    void setVertices(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    void setNormais(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    void setFaceNormal(const glm::vec3& n);

    inline void setColor(const glm::vec3& _color) { this->color = _color; }
    inline unsigned getSerial() const { return this->serial; }
    inline glm::vec3 getColor() const { return this->color; }
    inline glm::vec3* getVertices() { return this->vertices; }
    inline glm::vec3* getNormais() { return this->normais; }
    inline glm::vec3 getFaceNormal() const { return this->faceNormal; }

  private:
    void computeFaceNormalFromNormais();
    unsigned serial;
    static unsigned serialMaster;
    glm::vec3 faceNormal;
    glm::vec3 color;
    glm::vec3 vertices[3];
    glm::vec3 normais[3];
};
#endif
