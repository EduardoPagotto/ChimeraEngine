#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/TextureManager.hpp"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace Chimera {

void EmitterFont::recycleLife(const double& ts) {

    for (int ci = 0; ci < containers.size(); ci++) {
        pc = containers[ci];

        if (pc->count < pc->stop) {
            int newparticles = (int)(ts * 10000.0); // 160 particulas a 60 FPS
            if (newparticles > 160)
                newparticles = 160;

            int particleIndex = 0;
            for (int i = 0; i < newparticles; i++) {
                particleIndex = findUnusedParticle();
                if (particleIndex < 0) {
                    pc->count++;
                    if (pc->respaw)
                        particleIndex = 0;
                    else
                        break;
                }

                ParticleZ& p = pc->container[particleIndex];
                this->reset(p);
            }
        }

        // Simulate all particles
        int ParticlesCount = 0;
        for (uint32_t i = 0; i < pc->container.size(); i++) {
            ParticleZ& p = pc->container[i];
            if (p.life > 0) { // stil alive
                this->decrease(p, ts, ParticlesCount);
                ParticlesCount++;
            }
        }

        // Ordenar em relacao a posicao da camera, back to front
        std::sort(pc->container.begin(), pc->container.end());

        // printf("%d \n",ParticlesCount);
        pc->particlesCount = ParticlesCount;
    }
}
int EmitterFont::findUnusedParticle() {
    for (int i = pc->lastUsed; i < pc->container.size(); i++) {
        if (pc->container[i].life < 0) { // is dead
            pc->lastUsed = i;
            return i;
        }
    }

    for (int i = 0; i < pc->lastUsed; i++) {
        if (pc->container[i].life < 0) { // is dead
            pc->lastUsed = i;
            return i;
        }
    }
    return -1; // All particles are taken, wait new cicle
}

void EmitterFont::reset(ParticleZ& p) {

    glm::vec3 randomdir =
        glm::vec3((rand() % 2000 - 1000.0f) / 1000.0f, (rand() % 2000 - 1000.0f) / 1000.0f, (rand() % 2000 - 1000.0f) / 1000.0f);

    p.life = pc->life;
    p.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    p.speed = maindir + randomdir * spread;

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
        p.distance = glm::length2(p.pos - pc->cameraPos);

        // Fill the GPU buffer
        pc->posData[index] = glm::vec4(p.pos.x, p.pos.y, p.pos.z, p.size);

        pc->colorData[4 * index + 0] = p.color[0]; //.r; // p.r;
        pc->colorData[4 * index + 1] = p.color[1]; //.g; // p.g;
        pc->colorData[4 * index + 2] = p.color[2]; //.b; // p.b;
        pc->colorData[4 * index + 3] = p.color[3]; //.a; // p.a;

    } else {
        // Particles that just died will be put at the end of the buffer in
        // SortParticles();
        p.distance = -1.0f;
    }
}
} // namespace Chimera