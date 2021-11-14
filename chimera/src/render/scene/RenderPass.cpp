#include "chimera/render/scene/RenderPass.hpp"
#include "chimera/core/MouseDevice.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/bullet/Solid.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

RenderPass::RenderPass(uint32_t width, uint32_t height) {

    viewportWidth = width;
    viewportHeight = height;

    // Define o framebuffer de desenho
    Shader shader;
    ShaderManager::load("./assets/shaders/CanvasHMD.glsl", shader);

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = width;
    fbSpec.height = height;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    if (renderBuffer) {
        delete renderBuffer;
        renderBuffer = nullptr;
    }

    renderBuffer = new RenderBuffer(0, 0, new FrameBuffer(fbSpec), shader);
}

void RenderPass::execute(ICamera* camera, IRenderer3d& renderer, entt::registry& eRegistry) {

    // if (renderer.getLog() == true) {
    //     glm::vec3 pos = camera->getPosition();
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    // }

    renderer.begin(camera);

    // load lights after begin (clear previos lights)
    auto lightView = eRegistry.view<LightComponent>();
    for (auto entity : lightView) {
        auto& lc = lightView.get<LightComponent>(entity);
        if (lc.global) {
            renderer.submitLight(lc.light);
        }
    }

    renderer.submitUniform(UniformVal("projection", camera->getProjectionMatrix()));
    renderer.submitUniform(UniformVal("view", camera->getViewMatrix()));
    renderer.submitUniform(UniformVal("shadows", (int)0)); // FIXME: preciso disto aqui ?? quanto ativar a sombra passar para 1

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

        // Transform& tc = renderable->getEntity().getComponent<Transform>();
        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        command.renderable = renderable;
        // command.transform = tc.getMatrix();
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);

        command.uniforms.push_back(UniformVal("model", command.transform));

        rc.renderable->submit(camera, command, &renderer);
    }

    renderer.end();

    renderBuffer->bind(); // we're not using the stencil buffer now

    renderer.flush();

    // get val from color buffer (must be inside framebuffer renderer
    glm::ivec2 pos = MouseDevice::getMove();
    pos.y = viewportHeight - pos.y;
    int val = renderBuffer->getFramBuffer()->readPixel(1, pos.x, pos.y);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);

    renderBuffer->unbind();

    // // get val from color buffer (must be inside framebuffer renderer
    // glm::ivec2 pos = MouseDevice::getMove();
    // pos.y = viewportHeight - pos.y;
    // int val = fb->readPixel(1, pos.x, pos.y);
    // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);

    // VertexArray::unbind();
    // VertexBuffer::unbind();
    // Shader::disable();

    // renderBuffer->render();
}

} // namespace Chimera