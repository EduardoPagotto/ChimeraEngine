#pragma once
#include "chimera/base/ICamera.hpp"
#include "chimera/base/TransformationStack.hpp"
#include "chimera/base/Uniform.hpp"
#include "chimera/base/ViewProjection.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/space/Octree.hpp"
#include <vector>

namespace ce {

    class IRenderer3d;

    class Renderable3D {

      protected:
        uint32_t indexAuxCommand = 0;
        std::shared_ptr<VertexArray> vao;

      public:
        Renderable3D() = default;

        virtual ~Renderable3D() { vao.reset(); }

        virtual void draw(const bool& logData) {
            if (logData)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderable3D draw");
        }

        virtual const uint32_t getSize() const = 0;

        virtual const AABB& getAABB() const = 0;

        virtual std::shared_ptr<IndexBuffer> getIBO() const = 0;

        virtual void submit(RenderCommand& command, IRenderer3d& renderer) = 0;

        inline std::shared_ptr<VertexArray> getVao() const { return vao; }

        inline void setIndexAuxCommand(const uint32_t& command) { indexAuxCommand = command; }

        inline const uint32_t getIndexAuxCommand() const { return indexAuxCommand; }
    };

    class IRenderer3d {

      protected:
        Camera* camera = nullptr;
        ViewProjection* vpo = nullptr;
        TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
        MapUniform uniformsQueue;

      public:
        IRenderer3d() {
            uniformsQueue.reserve(500);
        } // FIXME: ViewProjection pode ser subistituido pela matix memso ???

        virtual ~IRenderer3d() = default;

        virtual void begin(Camera* camera, ViewProjection* vpo, Octree* octree) = 0;

        virtual void submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) = 0;

        virtual void end() = 0;

        virtual void flush() = 0;

        inline Camera* getCamera() const { return camera; }

        inline ViewProjection* getViewProjection() const { return vpo; }

        inline TransformationStack& getStack() { return stack; };

        inline MapUniform& uboQueue() { return uniformsQueue; }
    };
} // namespace ce
