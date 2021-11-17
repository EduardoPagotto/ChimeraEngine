#include "chimera/node/NodeParticleEmitter.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/VisitorInterface.hpp"
#include "chimera/render/TextureManager.hpp"
#include <algorithm>

namespace Chimera {
NodeParticleEmitter::NodeParticleEmitter(Node* _parent, std::string _name, int _max) : Node(_parent, Kind::PARTICLE_SYSTEM, _name) {
    LastUsedParticle = 0;
    material = new Material();
    particlesContainer.reserve(MaxParticles);
    for (int i = 0; i < MaxParticles; i++) {
        particlesContainer.push_back(Particle());
    }
}

NodeParticleEmitter::~NodeParticleEmitter() {}

void NodeParticleEmitter::init() {

    pTransform->init(glm::vec3());

    material->init();

    vao.bind();

    vPosSize = new glm::vec4[MaxParticles]; // buffer de posicoes de cada particula
    vColor = new GLubyte[MaxParticles * 4]; // buffer de cor de cada particula

    // The VBO containing the 4 vertices of the particles. Thanks to instancing, they will be shared by all particles.
    static const glm::vec3 vVertex[] = {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f),
                                        glm::vec3(0.5f, 0.5f, 0.0f)};

    // VBO square vertex
    vboVex = new VertexBuffer(BufferType::STATIC, sizeof(glm::vec3) * 4, (void*)vVertex);
    // The VBO containing the positions and sizes of the particles, is empty (NULL) buffer and it will be updated later, each frame.
    vboPos = new VertexBuffer(BufferType::STREAM, MaxParticles * sizeof(glm::vec4), nullptr); // set max val
    // The VBO containing the colors of the particles, is empty (NULL) buffer and it will be updated later, each frame.
    vboCor = new VertexBuffer(BufferType::STREAM, MaxParticles * 4 * sizeof(GLubyte), nullptr); // set max val

    timer.start();
}

void NodeParticleEmitter::loadTexDiffuse(const std::string& _nome, const std::string& _arquivo) {
    TextureManager::loadFromFile(_nome, _arquivo, TexParam());
    material->addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
}

void NodeParticleEmitter::accept(VisitorInterface* v) { v->visit(this); }

void NodeParticleEmitter::SortParticles() { std::sort(particlesContainer.begin(), particlesContainer.end()); }

int NodeParticleEmitter::recycleParticleLife(const glm::vec3& _camPosition) {

    double delta = timer.deltaTimeSecounds();

    // Generate 10 new particule each millisecond,
    // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
    // newparticles will be huge and the next frame even longer.
    int newparticles = (int)(delta * 10000.0);

    if (newparticles > (int)(0.016f * 10000.0))
        newparticles = (int)(0.016f * 10000.0);

    for (int i = 0; i < newparticles; i++) {

        int particleIndex = FindUnusedParticle();
        particlesContainer[particleIndex].reset();

        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 10.0f);

        particlesContainer[particleIndex].create(maindir, spread);
    }

    // Simulate all particles
    int ParticlesCount = 0;

    for (int i = 0; i < MaxParticles; i++) {

        Particle& particle = particlesContainer[i]; // shortcut
        if (particle.isDead() == false) {
            particle.decrease(delta, ParticlesCount, vPosSize, vColor, _camPosition);
            ParticlesCount++;
        }
    }

    SortParticles();
    // printf("%d \n", ParticlesCount);
    return ParticlesCount;
}

void NodeParticleEmitter::render(const Shader& shader) {
    //// We will need the camera's position in order to sort the particles
    //// w.r.t the camera's distance.
    //// There should be a getCameraPosition() function in common/controls.cpp,
    //// but this works too.
    // glm::vec3 CameraPosition(glm::inverse(_view)[3]);
    int ParticlesCount = recycleParticleLife(CameraPosition);

    // Update the buffers that OpenGL uses for rendering.
    // There are much more sophisticated means to stream data from the CPU to the GPU,
    // http://www.opengl.org/wiki/Buffer_Object_Streaming
    vao.bind();

    // Buffer orphaning, a common way to improve streaming, perf. See above link for details.
    vboPos->bind();
    vboPos->setSubData(vPosSize, 0, ParticlesCount * sizeof(glm::vec4));

    // Buffer orphaning, a common way to improve streaming, // perf. See above link for details.
    vboCor->bind();
    vboCor->setSubData(vColor, 0, ParticlesCount * sizeof(GLubyte) * 4);

    BinaryStateEnable depth(GL_DEPTH_TEST); // glEnable(GL_DEPTH_TEST);// Enable depth test
    BinaryStateEnable blender(GL_BLEND);    // glEnable(GL_BLEND);
    DepthFuncSetter depthFunc(GL_LESS);     // glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bind our texture
    material->bindMaterialInformation(shader);

    // 1rst attribute buffer : vertices
    vboVex->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : positions of particles' centers
    vboPos->bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3rd attribute buffer : particles' colors
    vboCor->bind();
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0); // normalized to float

    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to : for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

int NodeParticleEmitter::FindUnusedParticle() {

    for (int i = LastUsedParticle; i < MaxParticles; i++) {
        if (particlesContainer[i].isDead() == true) {
            LastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < LastUsedParticle; i++) {
        if (particlesContainer[i].isDead() == true) {
            LastUsedParticle = i;
            return i;
        }
    }

    return 0; // All particles are taken, override the first one
}
} // namespace Chimera
