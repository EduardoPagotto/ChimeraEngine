#include "chimera_render/3d/RenderableParticles.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"

namespace ce {

    RenderableParticles::~RenderableParticles() { this->destroy(); }

    void RenderableParticles::create() { // TODO: colocar os VBOs na extrutura vao!!!! usar AABB no local correto
        uint32_t max = pc_->max;
        vao = std::make_shared<VertexArray>();
        vao->bind();
        // The VBO containing the 4 vertices of the particles. Thanks to instancing, they will be shared by all
        // particles.
        static const glm::vec3 vVertex[] = {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f),
                                            glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f)};

        // VBO square vertex static, others (posiciton an size / color) is empty (NULL) buffer and it will be updated
        // later, each frame.
        vbo_vex_ = std::make_shared<VertexBuffer>(BufferType::STATIC, sizeof(glm::vec3) * 4, (void*)vVertex);
        vbo_pos_ = std::make_shared<VertexBuffer>(BufferType::STREAM, max * sizeof(glm::vec4), nullptr);
        vbo_cor_ = std::make_shared<VertexBuffer>(BufferType::STREAM, max * 4 * sizeof(GLubyte), nullptr);

        pc_->posData = new glm::vec4[max];     // buffer memoria de posicoes de cada particula
        pc_->colorData = new GLubyte[max * 4]; // buffer memoria de cor de cada particula

        pc_->container.reserve(max);
        for (int i = 0; i < max; i++) {
            pc_->container.push_back(ParticleZ());
        }
    }

    void RenderableParticles::destroy() {

        vbo_cor_.reset();
        vbo_pos_.reset();
        vbo_vex_.reset();

        if (pc_->posData) {
            delete[] pc_->posData;
            pc_->posData = nullptr;
        }

        if (pc_->colorData) {
            delete[] pc_->colorData;
            pc_->colorData = nullptr;
        }

        pc_->container.clear();

        vao.reset();
    }

    void RenderableParticles::submit(RenderCommand& command, IRenderer3d& renderer) {

        pc_->cameraPos =
            glm::inverse(renderer.getViewProjection()->getSel().view)[3]; // depois mover para o statemachine!!!
        renderer.submit(command, this, 0);
    }

    void RenderableParticles::draw(const bool& logData) {

        // particlesCount = recycleParticleLife();

        // Buffer orphaning, a common way to improve streaming, perf. See above link for details.
        vbo_pos_->bind();
        vbo_pos_->setSubData2(pc_->posData, 0, pc_->particlesCount * sizeof(glm::vec4)); // FIXME: usar o BuffewLayout

        // Buffer orphaning, a common way to improve streaming, // perf. See above link for details.
        vbo_cor_->bind();
        vbo_cor_->setSubData2(pc_->colorData, 0, pc_->particlesCount * sizeof(GLubyte) * 4);

        // Bind our texture
        // material->bindMaterialInformation(shader);
        // 1rst attribute buffer : vertices
        vbo_vex_->bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // 2nd attribute buffer : positions of particles' centers
        vbo_pos_->bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // 3rd attribute buffer : particles' colors
        vbo_cor_->bind();
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0); // normalized to float
        // These functions are specific to glDrawArrays*Instanced*.
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple
        // instances" http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad -> 1
        // Draw the particules !
        // This draws many times a small triangle_strip (which looks like a quad).
        // This is equivalent to : for(i in particlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
        // but faster.
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, pc_->particlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
} // namespace ce
