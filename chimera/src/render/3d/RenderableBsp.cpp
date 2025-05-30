#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/base/ICamera.hpp"
#include "chimera/core/partition/BSPTree.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"

namespace ce {

    RenderableBsp::RenderableBsp(Mesh& mesh) : Renderable3D(), totIndex(0) {

        Mesh meshFinal;
        meshReindex(mesh, meshFinal);

        BspTree bspTree;
        std::vector<TrisIndex> vTris;
        root = bspTree.create(meshFinal, vTris);

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
        vbo->setData(&meshFinal.vertex[0], meshFinal.vertex.size());
        vbo->unbind();

        vao->push(vbo);

        // Add all leafs and create IBO
        for (auto trisIndex : vTris) {

            auto [min, max, size] = vertexIndexedBoundaries(meshFinal.vertex, trisIndex);

            IndexBuffer* ibo = new IndexBuffer((uint32_t*)&trisIndex[0], trisIndex.size() * 3);
            IRenderable3d* r = new RenderableIBO(vao, ibo, AABB(min, max));
            vChild.push_back(r);
        }

        vao->unbind();

        auto [min, max, size] = vertexBoundaries(meshFinal.vertex);
        aabb.setBoundary(min, max);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", this->vChild.size());
    }

    RenderableBsp::~RenderableBsp() { this->destroy(); }

    void RenderableBsp::traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree,
                                     std::vector<IRenderable3d*>& childDraw) {
        // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
        if ((tree != nullptr) && (tree->isSolid == false)) {
            switch (SIDE result = tree->hyperPlane.classifyPoint(cameraPos); result) {
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
        traverseTree(cameraPos, root, childDraw);
        for (uint32_t c = 0; c < childDraw.size(); c++)
            renderer.submit(command, childDraw[c], c);

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
            tree->hyperPlane.intersect(Start, End, intersection, temp);
            return lineOfSight(Start, intersection, tree->front) && lineOfSight(End, intersection, tree->back);
        }

        if (PointA == SIDE::CP_BACK && PointB == SIDE::CP_FRONT) {
            tree->hyperPlane.intersect(Start, End, intersection, temp);
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
} // namespace ce
