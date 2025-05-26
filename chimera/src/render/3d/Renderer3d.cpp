#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/space/AABB.hpp"
#include <SDL3/SDL.h>

namespace ce {

    Renderer3d::Renderer3d(const bool& logData) : logData(logData) {
        vRenderable.reserve(500);
        vRenderCommand.reserve(50);
        textureQueue.reserve(32);
    }

    Renderer3d::~Renderer3d() {}

    void Renderer3d::begin(Camera* camera, ViewProjection* vpo, Octree* octree) {
        this->camera = camera;
        this->vpo = vpo;
        this->octree = octree;
        frustum.set(vpo->getSel().viewProjectionInverse);
    }

    void Renderer3d::end() {

        if (octree != nullptr) {
            std::queue<uint32_t> qIndexes;
            octree->visible(frustum, qIndexes);

            if (logData == true)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Octree Visible Indexes: %ld", qIndexes.size());

            while (!qIndexes.empty()) {
                qRenderableIndexes.push(qIndexes.front());
                qIndexes.pop();
            }
        }
    }

    void Renderer3d::submit(const RenderCommand& command, IRenderable3d* renderable, const uint32_t& count) {

        Renderable3D* r = (Renderable3D*)renderable;

        if (count == 0)
            vRenderCommand.push_back(command);

        r->setIndexAuxCommand(vRenderCommand.size() - 1);

        // Transformation model matrix AABB to know if in frustrum Camera
        const AABB& aabb = r->getAABB();
        AABB nova = aabb.transformation(command.transform);

        // Registro de todo AABB's com indice de Renderable3D
        if (this->octree != nullptr) {
            this->octree->insertAABB(nova, vRenderable.size());
        } else {
            // adicione apenas o que esta no clip-space
            if (nova.visible(frustum) == true) {
                qRenderableIndexes.push(vRenderable.size());
            }
        }

        vRenderable.push_back(r);
    }

    void Renderer3d::flush() {

        std::shared_ptr<Shader> activeShader = nullptr;
        VertexArray* pLastVao = nullptr;

        while (!qRenderableIndexes.empty()) {
            auto& r = vRenderable[qRenderableIndexes.front()];
            if (r->getVao() != pLastVao) { // Diferente  do anterior
                if (pLastVao != nullptr) { // desvincula o anterior
                    pLastVao->unbind();
                }

                const RenderCommand& command = vRenderCommand[r->getIndexAuxCommand()];
                r->getVao()->bind(); // vincula novo modelo
                pLastVao = r->getVao();

                if (activeShader == nullptr) { // primeira passada
                    activeShader = command.shader;
                    glUseProgram(activeShader->getID());
                } else {
                    // demais passadas
                    if ((*activeShader) != (*command.shader)) { // se diferente
                        if (command.shader != nullptr) {        // se valido trocar
                            glUseProgram(0);
                            activeShader = command.shader;
                            glUseProgram(activeShader->getID());
                        }
                    }
                }

                // generic bind in each draw call camera, light, etc
                for (const auto& kv : uniformsQueue)
                    activeShader->setUniformU(kv.first.c_str(), kv.second);

                // bind dos uniforms from model
                for (const auto& kv : command.uniforms)
                    activeShader->setUniformU(kv.first.c_str(), kv.second);

                // libera textura antes de passar as novas
                if (command.vTex.size() == 0)
                    Texture::unbind(0);

                // bind de texturas
                for (uint8_t i = 0; i < command.vTex.size(); i++)
                    command.vTex[i]->bind(i);

                // bind de texturas globais
                for (uint8_t i = 0; i < textureQueue.size(); i++)
                    textureQueue[i]->bind(command.vTex.size() + i);
            }

            r->draw(logData); // aqui

            qRenderableIndexes.pop();
        }

        pLastVao->unbind();

        uniformsQueue.clear();  // limpa comandos communs a todos VAO's
        textureQueue.clear();   // limpa fila de texturas
        vRenderable.clear();    // limpa array de desenho
        vRenderCommand.clear(); // Limpa rendercommand
    }

} // namespace ce
