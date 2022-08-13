#include "chimera/render/3d/Renderer3dLines.hpp"
#include "chimera/core/space/AABB.hpp"

namespace Chimera {

Renderer3dLines::Renderer3dLines() { drawLine = nullptr; }

Renderer3dLines::~Renderer3dLines() { destroy(); }

void Renderer3dLines::create(const uint32_t& sizeBuffer) {
    drawLine = new DrawLine();
    drawLine->create(sizeBuffer);
}

void Renderer3dLines::destroy() {
    if (drawLine != nullptr) {
        delete drawLine;
    }
}

void Renderer3dLines::begin(Camera* camera, ViewProjection* vpo, Octree* octree) {
    this->camera = camera;
    this->vpo = vpo;
    frustum.set(vpo->getViewProjectionInverse());
}

void Renderer3dLines::end() {}

bool Renderer3dLines::submit(const RenderCommand& command, IRenderable3d* renderable) {
    // Transformation model matrix AABB to know if in frustrum Camera
    Renderable3D* r = (Renderable3D*)renderable;
    const AABB& aabb = r->getAABB();
    AABB nova = aabb.transformation(command.transform);

    // adicione apenas o que esta no clip-space
    if (nova.visible(frustum) == true) {
        drawLine->addAABB(nova, glm::vec3(1.0, 1.0, 0.0)); // FIXME: ver se e esta cor mesmo
        return true;
    }

    return false;
}

void Renderer3dLines::flush() {
    drawLine->render(uniformsQueue);
    uniformsQueue.clear();
}
} // namespace Chimera