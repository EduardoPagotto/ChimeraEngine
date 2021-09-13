#include "chimera/render/bsp/BspTreeRender.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

BspTreeRender::BspTreeRender(BSPTreeNode* root, std::vector<VertexNode*>& vpLeafData, std::vector<VertexData>& vertexData) : root(root) {
    this->vpLeaf = std::move(vpLeafData);
    this->vVertex = std::move(vertexData);

    // create vertex buffers
    vao = new Core::VertexArray();
    vao->addBuffer(new Core::VertexBuffer(&this->vVertex[0], this->vVertex.size(), 3), 0); // FIXME: 0 por comatibilidade
    vao->bind();

    for (VertexNode* pLeaf : this->vpLeaf) {
        pLeaf->initAABB(&vVertex[0], vVertex.size()); // initialize AABB's
        pLeaf->initIndexBufferObject();               // create IBO's
        pLeaf->debugDados();
    }

    vao->unbind();

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Total Leaf: %ld", vpLeaf.size());
}

BspTreeRender::~BspTreeRender() { this->destroy(); }

void BspTreeRender::drawPolygon(BSPTreeNode* tree, bool frontSide, Frustum& _frustrun) {

    if (tree->isLeaf == false)
        return;

    auto pVn = vpLeaf[tree->leafIndex];
    if (pVn->getAABB()->visible(_frustrun) == true) {
        if (logdata == true)
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Leaf: %d Faces: %d", tree->leafIndex, pVn->getSize());

        pVn->render();
    }
}

void BspTreeRender::traverseTree(BSPTreeNode* tree, glm::vec3* pos, Frustum& _frustrun) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

    SIDE result = tree->hyperPlane.classifyPoint(pos);
    switch (result) {
        case SIDE::CP_FRONT:
            traverseTree(tree->back, pos, _frustrun);
            drawPolygon(tree, true, _frustrun);
            traverseTree(tree->front, pos, _frustrun);
            break;
        case SIDE::CP_BACK:
            traverseTree(tree->front, pos, _frustrun);
            drawPolygon(tree, false, _frustrun); // Elimina o render do back-face
            traverseTree(tree->back, pos, _frustrun);
            break;
        default: // SIDE::CP_ONPLANE
            // the eye point is on the partition hyperPlane...
            traverseTree(tree->front, pos, _frustrun);
            traverseTree(tree->back, pos, _frustrun);
            break;
    }
}

void BspTreeRender::render(glm::vec3* eye, Frustum& _frustrun, bool _logData) {

    vao->bind();

    logdata = _logData;
    traverseTree(root, eye, _frustrun);

    vao->unbind();
}

void BspTreeRender::renderAABB() {
    for (auto pLeaf : this->vpLeaf) {
        pLeaf->getAABB()->render();
    }
}

void BspTreeRender::destroy() {

    while (!vpLeaf.empty()) {

        VertexNode* pLeaf = vpLeaf.back();
        vpLeaf.pop_back();

        delete pLeaf;
        pLeaf = nullptr;
    }

    collapse(root);
}

void BspTreeRender::collapse(BSPTreeNode* tree) {

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

bool BspTreeRender::lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree) {
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