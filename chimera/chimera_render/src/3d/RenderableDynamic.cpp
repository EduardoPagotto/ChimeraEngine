#include "chimera_render/3d/RenderableDynamic.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_core/gl/buffer/VertexBuffer.hpp"

namespace ce {

    RenderableDynamic::RenderableDynamic(const uint32_t& max) : max_(max) {

        vao = std::make_shared<VertexArray>();

        vbo_ = std::make_shared<VertexBuffer>(BufferType::STREAM); //????
        vbo_->bind();

        BufferLayout layout;
        layout.push<float>(3, false);
        layout.push<float>(3, false);
        layout.push<float>(2, false);
        vbo_->set_layout(layout);
        vbo_->re_size(max);
        // vbo->setData(vertexData, vertexSize);
        // vbo->releaseAtributes();
        vbo_->unbind();
    }

    RenderableDynamic::~RenderableDynamic() {
        vao.reset();
        vbo_.reset();
    }

    void RenderableDynamic::render(VertexData* pVertice,
                                   const uint32_t& size) { // FIXME: ver como fazer!!!! falta dados
        vao->bind();
        vbo_->bind();
        // //int tot = size * sizeof(VertexData);
        // // glBufferData(GL_ARRAY_BUFFER, 5000, nullptr, GL_STREAM_DRAW);
        // vbo->reSize(size);
        // vbo->setData() ??? criar funcao com glBufferSubData!!!
        // glBufferSubData(GL_ARRAY_BUFFER, 0, tot, pVertice);
        // glDrawArrays(GL_TRIANGLES, 0, size);
        vbo_->unbind();
        vao->unbind();
    }
} // namespace ce
