#include "chimera/render/3d/RendererParticleEmitter.hpp"
#include "chimera/render/3d/RenderableParticleEmitter.hpp"
#include "chimera/render/buffer/VertexArray.hpp"

namespace Chimera {
RendererParticleEmitter::RendererParticleEmitter() {}
RendererParticleEmitter::~RendererParticleEmitter() {}

void RendererParticleEmitter::begin(ICamera* camera) {

    // inicializa state machine do opengl
    BinaryStateEnable depth(GL_DEPTH_TEST); // glEnable(GL_DEPTH_TEST);// Enable depth test
    BinaryStateEnable blender(GL_BLEND);    // glEnable(GL_BLEND);
    DepthFuncSetter depthFunc(GL_LESS);     // glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->camera = camera;
}
void RendererParticleEmitter::end() {}
void RendererParticleEmitter::submitLight(Light* light) {}
void RendererParticleEmitter::submit(const RenderCommand& command) {
    // adicionado ao proximo render
    commandQueue.push_back(command);
}
void RendererParticleEmitter::flush() {
    Shader activeShader;
    VertexArray* pLastVao = nullptr;
    while (!commandQueue.empty()) {

        const RenderCommand& command = commandQueue.front();
        RenderableParticleEmitter* r = (RenderableParticleEmitter*)command.renderable;

        if (r->getVao() != nullptr) {      // Possui um novo modelo
            if (r->getVao() != pLastVao) { // Diferente  do anterior
                if (pLastVao != nullptr) { // desvincula o anterior
                    pLastVao->unbind();
                }

                r->getVao()->bind(); // vincula novo modelo
                pLastVao = r->getVao();

                if (activeShader.isInvalid()) { // primeira passada
                    activeShader = command.shader;
                    activeShader.enable();
                } else {
                    // demais passadas
                    if (activeShader != command.shader) {  // se diferente
                        if (!command.shader.isInvalid()) { // se valido trocar
                            activeShader.disable();
                            activeShader = command.shader;
                            activeShader.enable();
                        }
                    }
                }

                // generic bind in each draw call camera, light, etc
                for (const UniformVal& uniform : uniformsQueue)
                    uniform.setUniform(activeShader);

                // bind dos uniforms from model
                for (const UniformVal& uniform : command.uniforms)
                    uniform.setUniform(activeShader);

                // TODO: ver se é assim mesmo!!!!
                if (command.vTex.size() == 0)
                    Texture::unbind(0);

                // bind de texturas
                for (uint8_t i = 0; i < command.vTex.size(); i++) {
                    command.vTex[i]->bind(i);
                }
            }
        }

        r->draw();
        // int ParticlesCount = recycleParticleLife(CameraPosition);

        commandQueue.pop_front();
    }

    pLastVao->unbind();

    // Limpa buffer de uniforms ao terminar todos os draws calls
    uniformsQueue.clear();
}

} // namespace Chimera