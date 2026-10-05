#pragma once
#include "chimera_base/aux/ICamera.hpp"
#include "chimera_base/aux/TransformationStack.hpp"
#include "chimera_base/aux/Uniform.hpp"
#include "chimera_base/aux/ViewProjection.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_core/gl/buffer/IndexBuffer.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_space/Octree.hpp"
#include <vector>

namespace ce {

    class IRenderer3d;

    class Renderable3D {
      public:
        Renderable3D() = default;

        virtual ~Renderable3D() { vao.reset(); }

        virtual void draw(const bool& log_data) {
            if (log_data)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderable3D draw");
        }

        virtual const uint32_t get_size() const = 0;

        virtual const AABB& get_aabb() const = 0;

        virtual std::shared_ptr<IndexBuffer> get_ibo() const = 0;

        virtual void submit(RenderCommand& command, IRenderer3d& renderer) = 0;

        inline std::shared_ptr<VertexArray> get_vao() const { return vao; }

        inline void set_index_aux_command(const uint32_t& command) { indexAuxCommand = command; }

        inline const uint32_t get_index_aux_command() const { return indexAuxCommand; }

      protected:
        uint32_t indexAuxCommand = 0;
        std::shared_ptr<VertexArray> vao;
    };

    class IRenderer3d {
      public:
        IRenderer3d() {
            uniformsQueue.reserve(500);
        } // FIXME: ViewProjection pode ser subistituido pela matrix mesmo ???

        virtual ~IRenderer3d() = default;

        virtual void begin(std::shared_ptr<Camera> camera, std::shared_ptr<ViewProjection>,
                           std::shared_ptr<Octree> octree) = 0;

        virtual void submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) = 0;

        virtual void end() = 0;

        virtual void flush() = 0;

        inline std::shared_ptr<Camera> get_camera() const { return camera; }

        inline std::shared_ptr<ViewProjection> get_view_projection() const { return vpo; }

        inline TransformationStack& get_stack() { return stack; };

        inline MapUniform& ubo_queue() { return uniformsQueue; }

      protected:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<ViewProjection> vpo;
        TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
        MapUniform uniformsQueue;
    };
} // namespace ce
