#include "chimera/render/vbs/RenderableBsp.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableBsp::RenderableBsp(BSPTreeNode* root, std::vector<RenderableIBO*>& vpLeafData, std::vector<VertexData>& vertexData)
    : root(root), totIndex(0) {

    this->vpLeaf = std::move(vpLeafData);
    this->vVertex = std::move(vertexData);

    // create vertex buffers
    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(&this->vVertex[0], this->vVertex.size(), 3), 0); // FIXME: 0 por comatibilidade
    vao->bind();

    uint32_t totIndex = 0;
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

RenderableBsp::~RenderableBsp() { this->destroy(); }

void RenderableBsp::drawPolygon(BSPTreeNode* tree, bool frontSide) {

    if (tree->isLeaf == false)
        return;

    auto pLeaf = vpLeaf[tree->leafIndex];
    pLeaf->inject(eye, frustum, logdata, renderQueue);
}

void RenderableBsp::traverseTree(BSPTreeNode* tree) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

    SIDE result = tree->hyperPlane.classifyPoint(eye);
    switch (result) {
        case SIDE::CP_FRONT:
            traverseTree(tree->back);
            drawPolygon(tree, true);
            traverseTree(tree->front);
            break;
        case SIDE::CP_BACK:
            traverseTree(tree->front);
            drawPolygon(tree, false); // Elimina o render do back-face
            traverseTree(tree->back);
            break;
        default: // SIDE::CP_ONPLANE
            // the eye point is on the partition hyperPlane...
            traverseTree(tree->front);
            traverseTree(tree->back);
            break;
    }
}

void RenderableBsp::inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) {
    this->eye = eye;
    this->frustum = frustum;
    this->renderQueue = renderQueue;
    this->logdata = logData;

    renderQueue->push_back(this);

    traverseTree(root);
}

void RenderableBsp::destroy() {

    while (!vpLeaf.empty()) {

        RenderableIBO* pLeaf = vpLeaf.back();
        vpLeaf.pop_back();

        delete pLeaf;
        pLeaf = nullptr;
    }

    collapse(root);
}

void RenderableBsp::collapse(BSPTreeNode* tree) {

    if (tree->front != nullptr) {
        collapse(tree->front);
        delete tree->front;
        tree->front = nullptr;
    }

    if (tree->back != nullptr) {
        collapse(tree->back);
        delete tree->back;
        tree->back = nullptr;
    }
}

bool RenderableBsp::lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
    float temp;
    glm::vec3 intersection;
    if (tree->isLeaf == true) {
        return !tree->isSolid;
    }

    SIDE PointA = tree->hyperPlane.classifyPoint(Start);
    SIDE PointB = tree->hyperPlane.classifyPoint(End);

    if (PointA == SIDE::CP_ONPLANE && PointB == SIDE::CP_ONPLANE) {
        return lineOfSight(Start, End, tree->front);
    }

    if (PointA == SIDE::CP_FRONT && PointB == SIDE::CP_BACK) {
        tree->hyperPlane.intersect(Start, End, &intersection, &temp);
        return lineOfSight(Start, &intersection, tree->front) && lineOfSight(End, &intersection, tree->back);
    }

    if (PointA == SIDE::CP_BACK && PointB == SIDE::CP_FRONT) {
        tree->hyperPlane.intersect(Start, End, &intersection, &temp);
        return lineOfSight(End, &intersection, tree->front) && lineOfSight(Start, &intersection, tree->back);
    }

    // if we get here one of the points is on the hyperPlane
    if (PointA == SIDE::CP_FRONT || PointB == SIDE::CP_FRONT) {
        return lineOfSight(Start, End, tree->front);
    } else {
        return lineOfSight(Start, End, tree->back);
    }
    return true;
}
} // namespace Chimera