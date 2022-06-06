#include "chimera/render/scene/ShadowPass.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

ShadowPass::ShadowPass(const uint32_t& width, const uint32_t& height, const glm::mat4& projection) {
    // Create ShadowPass
    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/ShadowMappingDepth.frag";
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/ShadowMappingDepth.vert";
    ShaderManager::load("ShadowMappingDepth", shadeData, this->shader);

    // Define o framebuffer de Shadow
    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)};

    fbSpec.width = 2048;
    fbSpec.height = 2048;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    this->shadowBuffer = new FrameBuffer(fbSpec);
    this->lightProjection = projection;
}

ShadowPass::~ShadowPass() { delete shadowBuffer; }

void ShadowPass::exec(Registry& registry, ICamera* camera, EyeView* eyeView, IRenderer3d& renderer, ITrans* origem, const bool& logRender) {

    renderer.begin(eyeView);
    {
        auto lightViewEnt = registry.get().view<LightComponent>();
        for (auto entity : lightViewEnt) {
            auto& lc = lightViewEnt.get<LightComponent>(entity);
            auto& tc = registry.get().get<TransComponent>(entity); // Lento
            if (lc.global) {
                // FIXME: usar o direcionm depois no segundo parametro
                glm::mat4 lightView = glm::lookAt(tc.trans->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
                this->lightSpaceMatrix = this->lightProjection * lightView;
                renderer.uQueue().insert(std::make_pair("lightSpaceMatrix", UValue(this->lightSpaceMatrix)));
            }
        }

        auto group = registry.get().group<TransComponent, Renderable3dComponent>();
        for (auto entity : group) {
            auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

            RenderCommand command;
            command.camera = camera;
            command.eyeView = eyeView;
            command.logRender = logRender;
            command.transform = tc.trans->translateSrc(origem->getPosition());
            command.renderable = rc.renderable;
            command.shader = this->shader;
            command.uniforms["model"] = UValue(command.transform);
            rc.renderable->submit(command, renderer);
        }
    }

    renderer.end();
    this->shadowBuffer->bind(); // we're not using the stencil buffer now

    renderer.flush();
    this->shadowBuffer->unbind();
}

void ShadowPass::appy(ICamera* camera, IRenderer3d& renderer) {
    renderer.uQueue().insert(std::make_pair("viewPos", UValue(camera->getPosition())));
    renderer.uQueue().insert(std::make_pair("shadows", UValue(1)));
    renderer.uQueue().insert(std::make_pair("shadowMap", UValue(1)));
    renderer.uQueue().insert(std::make_pair("lightSpaceMatrix", UValue(this->lightSpaceMatrix)));
    this->shadowBuffer->getDepthAttachemnt()->bind(1); // FIXME: ver como melhorar depois
}

} // namespace Chimera