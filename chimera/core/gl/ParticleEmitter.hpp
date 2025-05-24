#pragma once
#include "chimera/core/gl/OpenGLDefs.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/space/AABB.hpp"
#include <vector>

namespace ce {

    struct ParticleZ {
        glm::vec3 pos = glm::vec3(0.0f), speed = glm::vec3(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
        float size = 0.0f, life = -1.0f, distance = 0.0f;

        ParticleZ() = default;
        ParticleZ(const ParticleZ& o)
            : pos(o.pos), speed(o.speed), color(o.color), size(o.size), life(o.life), distance(o.distance) {}
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
        uint32_t stop = -1; // Onde parar
        uint32_t count = 0; // Contagem atual
        uint32_t max = 500; // Maximo de particulas
        bool respaw = true; // Resetar particula zero quando todas as particulas prontas
        float life = 2.0;   // Inicial do life
        AABB aabb;
    };

    class IEmitter {
      public:
        virtual int findUnusedParticle() = 0;
        virtual void reset(ParticleZ& p) = 0;
        virtual void recycleLife(const double& ts) = 0;
        virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) = 0;
        virtual void pushParticleContainer(ParticleContainer* pc) = 0;
        virtual ParticleContainer* getContainer(uint32_t pos) = 0;
    };

    class EmitterFont : public IEmitter {
      public:
        EmitterFont(const glm::vec3& dir, const float& spread) : pc(nullptr), maindir(dir), spread(spread) {};
        virtual int findUnusedParticle() override;
        virtual void reset(ParticleZ& p) override;
        virtual void recycleLife(const double& ts) override;
        virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) override;
        virtual void pushParticleContainer(ParticleContainer* pc) override { containers.push_back(pc); }
        virtual ParticleContainer* getContainer(uint32_t pos) override { return containers[pos]; }

      private:
        std::vector<ParticleContainer*> containers;
        ParticleContainer* pc = nullptr;
        glm::vec3 maindir;
        float spread;
    };

    struct EmitterComponent {
        IEmitter* emitter;
        TagComponent tag;
        EmitterComponent() : emitter(nullptr) {}
    };

} // namespace ce
