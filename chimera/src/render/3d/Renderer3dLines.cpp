#include "chimera/render/3d/Renderer3dLines.hpp"
#include "chimera/space/AABB.hpp"

namespace ce {

    void Renderer3dLines::begin(Camera* camera, ViewProjection* vpo, Octree* octree) {
        this->camera = camera;
        this->vpo = vpo;
        frustum.set(vpo->getSel().viewProjectionInverse);
    }

    void Renderer3dLines::end() {}

    void Renderer3dLines::submit(const RenderCommand& command, IRenderable3d* renderable, const uint32_t& count) {
        // Transformation model matrix AABB to know if in frustrum Camera
        Renderable3D* r = (Renderable3D*)renderable;
        const AABB& aabb = r->getAABB();
        AABB nova = aabb.transformation(command.transform);

        // adicione apenas o que esta no clip-space
        if (nova.visible(frustum) == true)
            drawLine.addAABB(nova, glm::vec3(1.0, 1.0, 0.0));
    }

    void Renderer3dLines::flush() {
        drawLine.render(uniformsQueue);
        uniformsQueue.clear();
    }
} // namespace ce
