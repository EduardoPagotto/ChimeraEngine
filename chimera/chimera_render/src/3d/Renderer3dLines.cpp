#include "chimera_render/3d/Renderer3dLines.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    void Renderer3dLines::begin(std::shared_ptr<Camera> camera, std::shared_ptr<ViewProjection> vpo,
                                std::shared_ptr<Octree> octree) {

        this->camera = camera;
        this->vpo = vpo;
        frustum_.set(vpo->get_sel().viewProjectionInverse);
    }

    void Renderer3dLines::end() {}

    void Renderer3dLines::submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) {
        // Transformation model matrix AABB to know if in frustrum Camera
        const AABB nova = renderable->getAABB().transformation(command.transform);

        // only if in the clip-space
        if (nova.visible(frustum_) == true)
            draw_line_.addAABB(nova, glm::vec3(1.0, 1.0, 0.0));
    }

    void Renderer3dLines::flush() {
        draw_line_.render(uniformsQueue);
        uniformsQueue.clear();
    }
} // namespace ce
