#include "chimera/render/vbs/RenderableIndex.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableIndex::RenderableIndex(std::vector<RenderableIBO*>& vpLeafData, std::vector<VertexData>& vertexData) : totIndex(0) {

    this->vpLeaf = std::move(vpLeafData);
    this->vVertex = std::move(vertexData);

    // create vertex buffers
    vao = new Core::VertexArray();
    vao->bind();

    Core::VertexBuffer* vbo = new Core::VertexBuffer(Core::BufferType::STATIC);
    vbo->bind();
    vbo->setLayout(vertexDataComponentes());
    vbo->setData(&this->vVertex[0], this->vVertex.size());
    vbo->unbind();

    vao->push(vbo);

    for (RenderableIBO* pLeaf : this->vpLeaf) {
        pLeaf->initAABB(&vVertex[0], vVertex.size()); // initialize AABB's
        pLeaf->initIndexBufferObject();               // create IBO's
        pLeaf->debugDados();
        totIndex += pLeaf->getSize();
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vVertex[0], vVertex.size(), min, max, size);
    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Total Leaf: %ld", vpLeaf.size());
}

RenderableIndex::~RenderableIndex() {
    vao->bind();
    // TODO: remover tudo
    // while (vVertex.size() > 0) {
    //     std::vector<RenderableIBO*>::iterator it = vVertex.begin();
    //     RenderableIBO* pNode = (*it);
    //     delete pNode;
    //     pNode = nullptr;
    //     vVertex.erase(it);
    // }
    vao->unbind();

    delete vao;
    vao = nullptr;
}

void RenderableIndex::debugDados() {
    // TODO:
}

void RenderableIndex::submit(IRenderer3d* renderer) {
    renderer->submit(this);

    for (RenderableIBO* pNode : vpLeaf) {
        renderer->submit(pNode);
    }
}
} // namespace Chimera