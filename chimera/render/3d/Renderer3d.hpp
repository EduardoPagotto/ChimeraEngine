#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/space/Frustum.hpp"
#include "chimera/space/Octree.hpp"

namespace ce {

    class Renderer3d : public IRenderer3d {

      private:
        std::queue<uint32_t> qRenderableIndexes;
        std::vector<RenderCommand> vRenderCommand;
        std::vector<Renderable3D*> vRenderable;
        std::vector<std::shared_ptr<Texture>> textureQueue;
        Frustum frustum;
        bool logData;
        Octree* octree;

      public:
        Renderer3d(const bool& logData);

        virtual ~Renderer3d();

        virtual void begin(std::shared_ptr<Camera> camera, ViewProjection* vpo, Octree* octree) override;

        virtual void submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) override;

        virtual void end() override;

        virtual void flush() override;

        virtual inline std::vector<std::shared_ptr<Texture>>& texQueue() { return textureQueue; }
    };
} // namespace ce
