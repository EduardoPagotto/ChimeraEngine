#include "Particle.hpp"

#include <stdlib.h>

#include "OpenGLDefs.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

namespace Chimera {
Particle::Particle() {

    life = -1.0f;
    cameradistance = -1.0f;
}
Particle::~Particle() {}

void Particle::create(const glm::vec3& _maindir, const float& _spread) {
    // Very bad way to generate a random direction;
    // See for instance
    // http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution
    // instead, combined with some user-controlled parameters (main direction, spread,
    // etc)
    glm::vec3 randomdir = glm::vec3((rand() % 2000 - 1000.0f) / 1000.0f,
                                    (rand() % 2000 - 1000.0f) / 1000.0f,
                                    (rand() % 2000 - 1000.0f) / 1000.0f);

    speed = _maindir + randomdir * _spread;

    // Very bad way to generate a random color
    color.set(rand() % 256, rand() % 256, rand() % 256,
              rand() % 256 / 3); // FIXME acho que esta errado, conferir em debug
    size = (rand() % 1000) / 2000.0f + 0.1f;
}

void Particle::decrease(const float& _delta, int _particlesCount,
                        GLfloat* pPositionSizeData, GLubyte* pColorData,
                        const glm::vec3& _cameraPosition) {
    // Decrease life
    life -= _delta;
    if (life > 0.0f) {

        // Simulate simple physics : gravity only, no collisions
        speed += glm::vec3(0.0f, 0.0f, -9.8f) * (float)_delta * 0.5f;
        pos += speed * (float)_delta; // *0.01f;
        cameradistance = glm::length2(pos - _cameraPosition);
        // ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

        // Fill the GPU buffer
        pPositionSizeData[4 * _particlesCount + 0] = pos.x;
        pPositionSizeData[4 * _particlesCount + 1] = pos.y;
        pPositionSizeData[4 * _particlesCount + 2] = pos.z;

        pPositionSizeData[4 * _particlesCount + 3] = size;

        pColorData[4 * _particlesCount + 0] = color.r; // p.r;
        pColorData[4 * _particlesCount + 1] = color.g; // p.g;
        pColorData[4 * _particlesCount + 2] = color.b; // p.b;
        pColorData[4 * _particlesCount + 3] = color.a; // p.a;

    } else {
        // Particles that just died will be put at the end of the buffer in
        // SortParticles();
        cameradistance = -1.0f;
    }
}

void Particle::reset() {
    life = 5.0f; // This particle will live 5 seconds.
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
} // namespace Chimera
