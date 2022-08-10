#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/core/partition/BSPTree.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {

RenderableBsp::RenderableBsp(Mesh* mesh) : totIndex(0), Renderable3D() {

    Mesh meshFinal;
    meshReCompile(*mesh, meshFinal);

    BspTree bspTree;
    std::vector<TrisIndex> vTris; // FIXME: limpar ??
    root = bspTree.create(&meshFinal, vTris);

    std::vector<VertexData> vVertex;
    meshFinal.serialized = true; // FIXME: sera ?????
    vertexDataFromMesh(&meshFinal, vVertex);

    // create VAO and VBO
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.Push<float>(3, false);
    layout.Push<float>(3, false);
    layout.Push<float>(2, false);

    vbo->setLayout(layout);
    vbo->setData(&vVertex[0], vVertex.size());
    vbo->unbind();

    vao->push(vbo);

    // Add all leafs and create IBO
    for (auto trisIndex : vTris) {

        glm::vec3 min, max, size;
        totIndex += trisIndex.size();
        vertexDataIndexMinMaxSize(&vVertex[0], vVertex.size(), &trisIndex.vIndex[0], trisIndex.size(), min, max, size);

        IndexBuffer* ibo = new IndexBuffer(&trisIndex.vIndex[0], trisIndex.size());
        IRenderable3d* r = new RenderableIBO(ibo, AABB(min, max));
        vChild.push_back(r);
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vVertex[0], vVertex.size(), min, max, size);
    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", this->vChild.size());
}

RenderableBsp::~RenderableBsp() { this->destroy(); }

void RenderableBsp::traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree, std::vector<IRenderable3d*>& childDraw) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if ((tree != nullptr) && (tree->isSolid == false)) {
        SIDE result = tree->hyperPlane.classifyPoint(cameraPos);
        switch (result) {
            case SIDE::CP_FRONT: {
                traverseTree(cameraPos, tree->back, childDraw);
                if (tree->isLeaf == true) // set to draw Polygon
                    childDraw.push_back(vChild[tree->leafIndex]);

                traverseTree(cameraPos, tree->front, childDraw);
            } break;
            case SIDE::CP_BACK: {
                traverseTree(cameraPos, tree->front, childDraw);
                if (tree->isLeaf == true) // set to draw Polygon
                    childDraw.push_back(vChild[tree->leafIndex]);

                traverseTree(cameraPos, tree->back, childDraw);
            } break;
            default: { // SIDE::CP_ONPLANE  // the eye point is on the partition hyperPlane...
                traverseTree(cameraPos, tree->front, childDraw);
                traverseTree(cameraPos, tree->back, childDraw);
            } break;
        }
    }
}

void RenderableBsp::submit(RenderCommand& command, IRenderer3d& renderer) {
    std::vector<IRenderable3d*> childDraw;
    const glm::vec3 cameraPos = renderer.getCamera()->getPosition();
    if (renderer.submit(command, this) == true)
        traverseTree(cameraPos, root, childDraw);

    for (IRenderable3d* child : childDraw)
        renderer.submit(command, child);

    childDraw.clear();
}

void RenderableBsp::destroy() {

    while (!vChild.empty()) {
        IRenderable3d* child = vChild.back();
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