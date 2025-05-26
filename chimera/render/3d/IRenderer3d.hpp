#pragma once
#include "chimera/base/ICamera.hpp"
#include "chimera/base/TransformationStack.hpp"
#include "chimera/base/Uniform.hpp"
#include <vector>

namespace ce {
    class IRenderer3d {
      public:
        IRenderer3d() { uniformsQueue.reserve(500); }
        virtual ~IRenderer3d() = default;
        virtual void begin(class Camera* camera, class ViewProjection* vpo, class Octree* octree) = 0;
        virtual void submit(const struct RenderCommand& command, class IRenderable3d* renderable,
                            const uint32_t& count) = 0;
        virtual void end() = 0;
        virtual void flush() = 0;

        inline Camera* getCamera() const { return camera; }
        inline ViewProjection* getViewProjection() const { return vpo; }
        inline TransformationStack& getStack() { return stack; };
        inline MapUniform& uboQueue() { return uniformsQueue; }

      protected:
        Camera* camera = nullptr;
        ViewProjection* vpo = nullptr;
        TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
        MapUniform uniformsQueue;
    };
} // namespace ce
