#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/TextureManager.hpp"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace Chimera {

void EmitterFont::recycleLife(const double& ts) {
    int newparticles = (int)(ts * 10000.0); // TODO: conferir esta estranho!!!
    if (newparticles > (int)(0.016f * 10000.0))
        newparticles = (int)(0.016f * 10000.0);

    for (int i = 0; i < newparticles; i++) {
        uint32_t particleIndex = findUnusedParticle();
        ParticleZ& p = pc->container[particleIndex];
        this->reset(p);
    }

    // Simulate all particles
    int ParticlesCount = 0;
    for (uint32_t i = 0; i < pc->container.size(); i++) {
        ParticleZ& p = pc->container[i];
        if (p.isDead() == false) {
            this->decrease(p, ts, ParticlesCount);
            ParticlesCount++;
        }
    }

    // Ordenar em relacao a posicao da camera, back to front
    std::sort(pc->container.begin(), pc->container.end());

    // printf("%d \n",ParticlesCount);
    pc->particlesCount = ParticlesCount;
}
int EmitterFont::findUnusedParticle() {
    for (int i = pc->lastUsed; i < pc->container.size(); i++) {
        if (pc->container[i].isDead() == true) {
            pc->lastUsed = i;
            return i;
        }
    }

    for (int i = 0; i < pc->lastUsed; i++) {
        if (pc->container[i].isDead() == true) {
            pc->lastUsed = i;
            return i;
        }
    }
    return 0; // All particles are taken, override the first one
}

void EmitterFont::reset(ParticleZ& p) {

    glm::vec3 randomdir =
        glm::vec3((rand() % 2000 - 1000.0f) / 1000.0f, (rand() % 2000 - 1000.0f) / 1000.0f, (rand() % 2000 - 1000.0f) / 1000.0f);

    p.life = 5.0;
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

void EmitterFont::decrease(ParticleZ& p, const float& tsDelta, const uint32_t& index) {
    // Decrease life
    p.life -= tsDelta;
    if (p.life > 0.0f) {

        // Simulate simple physics : gravity only, no collisions
        p.speed += glm::vec3(0.0f, 0.0f, -9.8f) * (float)tsDelta * 0.5f;
        p.pos += p.speed * (float)tsDelta; // *0.01f;
        p.distance = glm::length2(p.pos - pc->cameraPos);
        // ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

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