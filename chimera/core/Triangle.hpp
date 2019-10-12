
#ifndef __TRIANGLE__HPP
#define __TRIANGLE__HPP

#include <glm/glm.hpp>

struct VertexDataFull {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec3 color;    // 3 * 4 = 12 (24 - 35)
    glm::vec2 texture;  // 2 * 4 = 08 (36 - 43) //cuidado, posicao muda o sizeof(VertexData)
};

class Triangle {
  public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    inline unsigned getSerial() const { return serial; }
    glm::vec3 normal();
    // glm::vec3 verticeNormal();
    VertexDataFull vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};
#endif
