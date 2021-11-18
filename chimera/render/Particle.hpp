#pragma once
#include "chimera/render/OpenGLDefs.hpp"
#include "glm/glm.hpp"

namespace Chimera {
class Particle {
  public:
    Particle();
    virtual ~Particle();
    void create(const glm::vec3& _maindir, const float& _spread);
    void decrease(const float& _delta, int _particlesCount, glm::vec4* pPositionSizeData, GLubyte* pColorData,
                  const glm::vec3& _cameraPosition);
    void reset();
    bool isDead() { return (life < 0); }
    bool operator<(const Particle& that) const {
        return this->distance > that.distance; // Sort in reverse order : far particles drawn first.
    }

  private:
    glm::vec3 pos, speed;
    glm::vec4 color;
    float size, life, distance;
};
} // namespace Chimera
