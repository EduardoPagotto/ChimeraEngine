#include "chimera_render/3d/Renderer3d.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_space/AABB.hpp"
#include <SDL3/SDL.h>

namespace ce {

    Renderer3d::Renderer3d(const bool& log_data) : log_data_(log_data) {
        v_renderable_.reserve(500);
        v_render_command_.reserve(50);
        texture_queue_.reserve(32);
    }

    void Renderer3d::begin(std::shared_ptr<Camera> camera, std::shared_ptr<ViewProjection> vpo,
                           std::shared_ptr<Octree> octree) {

        this->camera = camera;
        this->vpo = vpo;
        this->octree_ = octree;
        frustum_.set(vpo->get_sel().viewProjectionInverse);
    }

    void Renderer3d::end() {

        if (octree_ != nullptr) {
            std::queue<uint32_t> q_indexes;
            octree_->visible(frustum_, q_indexes);

            if (log_data_) {
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Octree Visible Indexes: %ld", q_indexes.size());
            }

            while (!q_indexes.empty()) {
                q_renderable_indexes_.push(q_indexes.front());
                q_indexes.pop();
            }
        }
    }

    void Renderer3d::submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) {

        if (count == 0) {
            v_render_command_.push_back(command);
        }

        renderable->set_index_aux_command(v_render_command_.size() - 1);

        // Transformation model matrix AABB to know if in frustrum Camera
        AABB nova = renderable->get_aabb().transformation(command.transform);

        // Registro de todo AABB's com indice de Renderable3D
        if (this->octree_ != nullptr) {
            this->octree_->insert_aabb(nova, v_renderable_.size());
        } else {
            // adicione apenas o que esta no clip-space
            if (nova.visible(frustum_)) {
                q_renderable_indexes_.push(v_renderable_.size());
            }
        }

        v_renderable_.push_back(renderable);
    }

    void Renderer3d::flush() {

        std::shared_ptr<Shader> active_shader;
        std::shared_ptr<VertexArray> p_last_vao;

        while (!q_renderable_indexes_.empty()) {
            auto& r = v_renderable_[q_renderable_indexes_.front()];
            if (r->get_vao() != p_last_vao) { // Diferente  do anterior
                if (p_last_vao != nullptr) {  // desvincula o anterior
                    p_last_vao->unbind();
                }

                const RenderCommand& command = v_render_command_[r->get_index_aux_command()];
                r->get_vao()->bind(); // vincula novo modelo
                p_last_vao = r->get_vao();

                if (active_shader == nullptr) { // primeira passada
                    active_shader = command.shader;
                    glUseProgram(active_shader->get_id());
                } else {
                    // demais passadas
                    if ((*active_shader) != (*command.shader)) { // se diferente
                        if (command.shader != nullptr) {         // se valido trocar
                            glUseProgram(0);
                            active_shader = command.shader;
                            glUseProgram(active_shader->get_id());
                        }
                    }
                }

                // generic bind in each draw call camera, light, etc
                for (const auto& kv : uniformsQueue) {
                    active_shader->set_uniform_u(kv.first.c_str(), kv.second);
                }

                // bind dos uniforms from model
                for (const auto& kv : command.uniforms) {
                    active_shader->set_uniform_u(kv.first.c_str(), kv.second);
                }

                // libera textura antes de passar as novas
                if (command.vTex.size() == 0) {
                    Texture::unbind(0);
                }

                // bind de texturas
                for (uint8_t i = 0; i < command.vTex.size(); i++) {
                    command.vTex[i]->bind(i);
                }

                // bind de texturas globais
                for (uint8_t i = 0; i < texture_queue_.size(); i++) {
                    texture_queue_[i]->bind(command.vTex.size() + i);
                }
            }

            r->draw(log_data_); // aqui

            q_renderable_indexes_.pop();
        }

        p_last_vao->unbind();

        uniformsQueue.clear();     // limpa comandos communs a todos VAO's
        texture_queue_.clear();    // limpa fila de texturas
        v_renderable_.clear();     // limpa array de desenho
        v_render_command_.clear(); // Limpa rendercommand
    }

} // namespace ce
