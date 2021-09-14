#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/RenderableIndex.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableIndex::RenderableIndex(std::vector<RenderableIBO*>& vpLeafData, std::vector<VertexData>& vertexData) : totIndex(0) {

    this->vpLeaf = std::move(vpLeafData);
    this->vVertex = std::move(vertexData);

    // create vertex buffers
    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(&this->vVertex[0], this->vVertex.size(), 3), 0); // FIXME: 0 por comatibilidade
    vao->bind();

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

void RenderableIndex::inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) {
    renderQueue->push_back(this);

    unsigned int tot = 0;

    for (RenderableIBO* pNode : vpLeaf) {
        bool teste = pNode->getAABB()->visible(*frustum);
        if (teste == true) {
            pNode->inject(eye, frustum, false, renderQueue);
            tot++;
        }
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "TOT: (%d)", tot);
}
} // namespace Chimera