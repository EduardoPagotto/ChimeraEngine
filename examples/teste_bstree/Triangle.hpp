
#ifndef __TRIANGLE__HPP
#define __TRIANGLE__HPP

#include <glm/glm.hpp>

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 (0 -11)
    glm::vec3 normal;   // 3 * 4 = 12 (12-23)
    glm::vec2 texture;  // 2 * 4 = 08 (24-31)
    glm::vec4 color;    // 4 * 4 = 16 (32-47) //cuidado com a posicao, muda o
    // sizeof(VertexData)
};

class Triangle {
  public:
    Triangle();
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    inline unsigned getSerial() const { return serial; }
    glm::vec3 normal();
    glm::vec3 verticeNormal();
    VertexData vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};
#endif
