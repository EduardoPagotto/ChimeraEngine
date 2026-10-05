#pragma once
// #pragma clang diagnostic ignored "-Wunused-private-field"
#include "IRenderer3d.hpp"
#include "chimera_core/visible/Mesh.hpp"

namespace ce {

    class RenderableDynamic : public Renderable3D {
      public:
        RenderableDynamic(const uint32_t& max);

        virtual ~RenderableDynamic();

        void render(VertexData* p_vertice, const uint32_t& size);

      private:
        uint32_t max_;
        std::shared_ptr<VertexBuffer> vbo_;
    };
} // namespace ce
