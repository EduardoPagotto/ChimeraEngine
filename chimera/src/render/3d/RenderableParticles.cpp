#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"

namespace Chimera {

RenderableParticles::~RenderableParticles() { this->destroy(); }

void RenderableParticles::create() { // TODO: colocar os VBOs na extrutura vao!!!! usar AABB no local correto
    uint32_t max = pc->max;
    vao = new VertexArray();
    vao->bind();
    // The VBO containing the 4 vertices of the particles. Thanks to instancing, they will be shared by all particles.
    static const glm::vec3 vVertex[] = {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f),
                                        glm::vec3(0.5f, 0.5f, 0.0f)};

    // VBO square vertex static, others (posiciton an size / color) is empty (NULL) buffer and it will be updated later, each frame.
    vboVex = new VertexBuffer(BufferType::STATIC, sizeof(glm::vec3) * 4, (void*)vVertex);
    vboPos = new VertexBuffer(BufferType::STREAM, max * sizeof(glm::vec4), nullptr);
    vboCor = new VertexBuffer(BufferType::STREAM, max * 4 * sizeof(GLubyte), nullptr);

    pc->posData = new glm::vec4[max];     // buffer memoria de posicoes de cada particula
    pc->colorData = new GLubyte[max * 4]; // buffer memoria de cor de cada particula

    pc->container.reserve(max);
    for (int i = 0; i < max; i++) {
        pc->container.push_back(ParticleZ());
    }
}

void RenderableParticles::destroy() {

    if (vboCor) {
        delete vboCor;
        vboCor = nullptr;
    }

    if (vboPos) {
        delete vboPos;
        vboPos = nullptr;
    }

    if (vboVex) {
        delete vboVex;
        vboVex = nullptr;
    }

    if (pc->posData) {
        delete[] pc->posData;
        pc->posData = nullptr;
    }

    if (pc->colorData) {
        delete[] pc->colorData;
        pc->colorData = nullptr;
    }

    pc->container.clear();

    if (vao) {
        delete vao;
        vao = nullptr;
    }
}

void RenderableParticles::submit(RenderCommand& command, IRenderer3d& renderer) {

    pc->cameraPos = glm::inverse(renderer.getViewProjection()->getView())[3]; // depois mover para o statemachine!!!
    renderer.submit(command, this, true);
}

void RenderableParticles::draw(const bool& logData) {

    // particlesCount = recycleParticleLife();

    // Buffer orphaning, a common way to improve streaming, perf. See above link for details.
    vboPos->bind();
    vboPos->setSubData2(pc->posData, 0, pc->particlesCount * sizeof(glm::vec4)); // FIXME: usar o BuffewLayout

    // Buffer orphaning, a common way to improve streaming, // perf. See above link for details.
    vboCor->bind();
    vboCor->setSubData2(pc->colorData, 0, pc->particlesCount * sizeof(GLubyte) * 4);

    // Bind our texture
    // material->bindMaterialInformation(shader);
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
    // This is equivalent to : for(i in particlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, pc->particlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
} // namespace Chimera
