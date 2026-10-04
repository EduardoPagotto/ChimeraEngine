#pragma once
#include "chimera_space/AABB.hpp"
#include <memory>
#include <vector>

namespace ce {

    struct ParticleZ {
        glm::vec3 pos = glm::vec3(0.0f), speed = glm::vec3(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
        float size = 0.0f, life = -1.0f, distance = 0.0f;

        ParticleZ() = default;
        ParticleZ(const ParticleZ& o) = default;
        bool operator<(const ParticleZ& that) const {
            return this->distance > that.distance; // Sort in reverse order : far particles drawn first.
        }
    };

    struct ParticleContainer {
        int lastUsed = 0;
        int particlesCount = 0;
        glm::vec4* posData = nullptr;
        unsigned char* colorData = nullptr;
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
        virtual int find_unused_particle() = 0;
        virtual void reset(ParticleZ& p) = 0;
        virtual void recycle_life(const double& ts) = 0;
        virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) = 0;
        virtual void push_particle_container(std::shared_ptr<ParticleContainer> pc) = 0;
        virtual std::shared_ptr<ParticleContainer> get_container(uint32_t pos) = 0;
    };

    class EmitterFont : public IEmitter {
      public:
        EmitterFont(const glm::vec3& dir, const float& spread) : pc_(nullptr), maindir_(dir), spread_(spread) {};
        virtual int find_unused_particle() override;
        virtual void reset(ParticleZ& p) override;
        virtual void recycle_life(const double& ts) override;
        virtual void decrease(ParticleZ& p, const double& ts, const uint32_t& index) override;
        virtual void push_particle_container(std::shared_ptr<ParticleContainer> pc) override {
            containers_.push_back(pc);
        }
        virtual std::shared_ptr<ParticleContainer> get_container(uint32_t pos) override { return containers_[pos]; }

      private:
        std::vector<std::shared_ptr<ParticleContainer>> containers_;
        std::shared_ptr<ParticleContainer> pc_;
        glm::vec3 maindir_;
        float spread_;
    };
} // namespace ce
