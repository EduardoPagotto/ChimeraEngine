#pragma once
#include "chimera/render/ICamera.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <vector>

namespace Chimera {

struct ParticleZ {
    glm::vec3 pos = glm::vec3(0.0f), speed = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(0.0f);
    float size = 0.0f, life = -1.0f, distance = 0.0f;

    ParticleZ() = default;
    ParticleZ(const ParticleZ& o) : pos(o.pos), speed(o.speed), color(o.color), size(o.size), life(o.life), distance(o.distance) {}
    bool operator<(const ParticleZ& that) const {
        return this->distance > that.distance; // Sort in reverse order : far particles drawn first.
    }
};

struct ParticleContainer {
    int lastUsed = 0;
    int particlesCount = 0;
    glm::vec4* posData = nullptr;
    GLubyte* colorData = nullptr;
    glm::vec3 cameraPos = glm::vec3(0.0f);
    std::vector<ParticleZ> container;
    ParticleContainer() = default;
};

class IEmitter {
  public:
    virtual int findUnusedParticle() = 0;
    virtual void reset(ParticleZ& p) = 0;
    virtual void recycleLife(const double& ts) = 0;
    virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) = 0;
    virtual void setParticleContainer(ParticleContainer* pc) = 0;
};

class EmitterFont : public IEmitter {
  public:
    EmitterFont() = default;
    virtual int findUnusedParticle() override;
    virtual void reset(ParticleZ& p) override;
    virtual void recycleLife(const double& ts) override;
    virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) override;
    virtual void setParticleContainer(ParticleContainer* pc) override { this->pc = pc; }

  private:
    ParticleContainer* pc = nullptr;
    glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 10.0f); // TODO: setar
    float spread = 1.5f;                              // TODO: setar
};
} // namespace Chimera