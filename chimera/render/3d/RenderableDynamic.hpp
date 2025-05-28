#pragma once
// #pragma clang diagnostic ignored "-Wunused-private-field"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"

namespace ce {

    class RenderableDynamic : public Renderable3D {

      private:
        [[maybe_unused]]
        uint32_t max;
        VertexBuffer* vbo;

      public:
        RenderableDynamic(const uint32_t& max);

        virtual ~RenderableDynamic();

        void render(VertexData* pVertice, const uint32_t& size);
    };
} // namespace ce
