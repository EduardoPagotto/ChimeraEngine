#include "chimera/render/scene/RenderPassShadow.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/bullet/Solid.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

RenderPassShadow::RenderPassShadow(uint32_t width, uint32_t height) {

    // Define o framebuffer de Shadow
    ShaderManager::load("./assets/shaders/ShadowMappingDepth.glsl", shader);

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)};

    fbSpec.width = width;   // 2048;
    fbSpec.height = height; // 2048;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    shadowBuffer = new FrameBuffer(fbSpec);
    lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 150.0f);
    // Note that if you use a
    // glm::mat4 lightProjection = glm::perspective(45.0f, (float)width / (float)height, near_plane, far_plane);
    // perspective projection matrix you'll have to change the light position as the
    // current light position isn't enough to reflect the whole scene.
}

void RenderPassShadow::execute(ICamera* camera, IRenderer3d& renderer, entt::registry& eRegistry) {

    renderer.begin(camera);

    // load lights after begin (clear previos lights)
    auto lightViewEnt = eRegistry.view<LightComponent>();
    for (auto entity : lightViewEnt) {
        auto& lc = lightViewEnt.get<LightComponent>(entity);
        if (lc.global) {
            // TODO: usar o direcionm depois no segundo parametro
            glm::mat4 lightView = glm::lookAt(lc.light->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
            this->lightSpaceMatrix = lightProjection * lightView;
            renderer.submitUniform(UniformVal("lightSpaceMatrix", lightSpaceMatrix));
        }
    }

    auto view = eRegistry.view<Renderable3dComponent>();
    for (auto entity : view) {

        Renderable3dComponent& rc = view.get<Renderable3dComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        RenderCommand command;

        if (renderable->getEntity().hasComponent<Solid>()) {
            Solid& sl = renderable->getEntity().getComponent<Solid>();
            command.transform = sl.getMatrix();
        } else {
            Transform& tc = renderable->getEntity().getComponent<Transform>();
            command.transform = tc.getMatrix();
        }

        command.renderable = renderable;
        command.shader = shader;
        command.uniforms.push_back(UniformVal("model", command.transform));

        rc.renderable->submit(camera, command, &renderer);
    }

    renderer.end();

    shadowBuffer->bind(); // we're not using the stencil buffer now

    renderer.flush();

    shadowBuffer->unbind();
}

} // namespace Chimera