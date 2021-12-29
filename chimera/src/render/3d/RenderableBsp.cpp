#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableBsp::RenderableBsp(BSPTreeNode* root, std::vector<Renderable3D*>* vChild, std::vector<VertexData>* vertexData)
    : root(root), totIndex(0) {

    this->vChild = std::move(*vChild);
    this->vVertex = std::move(*vertexData);

    // create vertex buffers
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);

    vbo->setLayout(layout);
    vbo->setData(&this->vVertex[0], this->vVertex.size());
    vbo->unbind();

    uint32_t totIndex = 0;
    for (Renderable3D* child : this->vChild) {

        child->initializeBuffer(&vVertex[0], vVertex.size());
        child->debugDados();
        totIndex += child->getSize();
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vVertex[0], vVertex.size(), min, max, size);
    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Total Leaf: %ld", this->vChild.size());
}

RenderableBsp::~RenderableBsp() { this->destroy(); }

void RenderableBsp::drawPolygon(BSPTreeNode* tree, bool frontSide) {

    if (tree->isLeaf == false)
        return;

    auto child = vChild[tree->leafIndex];
    command->renderable = child;
    child->submit(camera, *command, this->renderer);
}

void RenderableBsp::traverseTree(BSPTreeNode* tree) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    if (tree->isSolid == true)
        return;

    SIDE result = tree->hyperPlane.classifyPoint(camera->getPosition());
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

void RenderableBsp::debugDados() const { SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "BSP Submit"); }

void RenderableBsp::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) {
    this->camera = camera;
    this->renderer = renderer;
    this->command = &command;

    renderer->submit(command);

    // submit tree
    traverseTree(root);
}

void RenderableBsp::destroy() {

    while (!vChild.empty()) {

        Renderable3D* child = vChild.back();
        vChild.pop_back();

        delete child;
        child = nullptr;
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

bool RenderableBsp::lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree) {
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
        return lineOfSight(Start, intersection, tree->front) && lineOfSight(End, intersection, tree->back);
    }

    if (PointA == SIDE::CP_BACK && PointB == SIDE::CP_FRONT) {
        tree->hyperPlane.intersect(Start, End, &intersection, &temp);
        return lineOfSight(End, intersection, tree->front) && lineOfSight(Start, intersection, tree->back);
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