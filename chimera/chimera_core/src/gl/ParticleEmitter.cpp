#include "chimera_core/gl/ParticleEmitter.hpp"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace ce {

    void EmitterFont::recycle_life(const double& ts) {

        for (int ci = 0; ci < containers_.size(); ci++) {
            pc_ = containers_[ci];

            if (pc_->count < pc_->stop) {
                int newparticles = (int)(ts * 10000.0); // 160 particulas a 60 FPS
                if (newparticles > 160)
                    newparticles = 160;

                int particleIndex = 0;
                for (int i = 0; i < newparticles; i++) {
                    particleIndex = find_unused_particle();
                    if (particleIndex < 0) {
                        pc_->count++;
                        if (pc_->respaw)
                            particleIndex = 0;
                        else
                            break;
                    }

                    ParticleZ& p = pc_->container[particleIndex];
                    this->reset(p);
                }
            }

            // Simulate all particles
            int ParticlesCount = 0;
            glm::vec3 min = glm::vec3(0.0f);
            glm::vec3 max = glm::vec3(0.0f);
            for (uint32_t i = 0; i < pc_->container.size(); i++) {
                ParticleZ& p = pc_->container[i];
                if (p.life > 0) { // stil alive
                    this->decrease(p, ts, ParticlesCount);
                    ParticlesCount++;

                    // computa os bonderys
                    min = glm::min(min, p.pos);
                    max = glm::max(max, p.pos);
                }
            }
            // TODO: continuar o AABB sem necessidade de emisor
            pc_->aabb.set_boundary(min, max);

            // Ordenar em relacao a posicao da camera, back to front
            std::sort(pc_->container.begin(), pc_->container.end());

            // printf("%d \n",ParticlesCount);
            pc_->particlesCount = ParticlesCount;
        }
    }
    int EmitterFont::find_unused_particle() {
        for (int i = pc_->lastUsed; i < pc_->container.size(); i++) {
            if (pc_->container[i].life < 0) { // is dead
                pc_->lastUsed = i;
                return i;
            }
        }

        for (int i = 0; i < pc_->lastUsed; i++) {
            if (pc_->container[i].life < 0) { // is dead
                pc_->lastUsed = i;
                return i;
            }
        }
        return -1; // All particles are taken, wait new cicle
    }

    void EmitterFont::reset(ParticleZ& p) {

        glm::vec3 randomdir = glm::vec3((rand() % 2000 - 1000.0f) / 1000.0f, (rand() % 2000 - 1000.0f) / 1000.0f,
                                        (rand() % 2000 - 1000.0f) / 1000.0f);

        p.life = pc_->life;
        p.pos = glm::vec3(0.0f, 0.0f, 0.0f);
        p.speed = maindir_ + randomdir * spread_;

        // Very bad way to generate a random color
        // color.set(rand() % 256, rand() % 256, rand() % 256, rand() % 256 / 3);
        p.color.r = rand() % 256;
        p.color.g = rand() % 256;
        p.color.b = rand() % 256;
        p.color.a = rand() % 256 / 3;
        p.size = (rand() % 1000) / 2000.0f + 0.1f;
    }

    void EmitterFont::decrease(ParticleZ& p, const double& ts, const uint32_t& index) {
        // Decrease life
        p.life -= ts;
        if (p.life > 0.0f) {

            // Simulate simple physics : gravity only, no collisions
            p.speed += glm::vec3(0.0f, 0.0f, -9.8f) * (float)ts * 0.5f;
            p.pos += p.speed * (float)ts; // *0.01f;
            p.distance = glm::length2(p.pos - pc_->cameraPos);

            // Fill the GPU buffer
            pc_->posData[index] = glm::vec4(p.pos.x, p.pos.y, p.pos.z, p.size);

            pc_->colorData[4 * index + 0] = p.color[0]; //.r; // p.r;
            pc_->colorData[4 * index + 1] = p.color[1]; //.g; // p.g;
            pc_->colorData[4 * index + 2] = p.color[2]; //.b; // p.b;
            pc_->colorData[4 * index + 3] = p.color[3]; //.a; // p.a;

        } else {
            // Particles that just died will be put at the end of the buffer in
            // SortParticles();
            p.distance = -1.0f;
        }
    }
} // namespace ce
