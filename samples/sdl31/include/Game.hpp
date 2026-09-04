#pragma once
#include "MeshModel.hpp"
#include "cevk/Frame.hpp"
#include "cevk/Pipeline.hpp"
#include "cevk/RenderPass.hpp"
#include "cevk/SwapChain.hpp"
#include "cevk/Textures.hpp"
#include "cevk/UBO.hpp"
#include "cevk/VulkanContext.hpp"
#include "cevk_engine/ScreenVK.hpp"
#include "cevk_infra/IStateMachine.hpp"
#include <memory>

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<ce::VulkanContext> ctx, std::shared_ptr<ce::ScreenVK> screen);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override;

  private:
    // - Vulkan create functions
    void createDescriptorSetLayout();
    void createPushConstantRange();
    void createGraphicsPipeline();
    void createDescriptorPool();
    void createDescriptorSets();

    void updateModel(int modelId, glm::mat4 newModel);
    int createMeshModel(const std::string& modelFile);

    void draw();

    int currentFrame{0};

    std::shared_ptr<ce::VulkanContext> ctx;

    VkPushConstantRange pushConstantRange;

    // Scene Settings
    struct UboViewProjection {
        glm::mat4 projection;
        glm::mat4 view;
    } uboViewProjection;

    ce::DescriptorPool descriptorPool;
    ce::UniformBuffer uniformBufferVP;

    ce::RenderPass renderPass;
    ce::SwapChain swapchain;
    std::vector<VkClearValue> clearValues;

    std::vector<ce::Frame> frames;

    std::shared_ptr<ce::Textures> textureMng;
    std::shared_ptr<ce::PipelineLayout> pipelineLayout;
    std::shared_ptr<ce::Pipeline> graphicPipeline;

    // Scene Objects
    std::vector<ce::MeshModel> modelList;

    //
    std::shared_ptr<ce::ScreenVK> screen;
    //
    float angle{0.0F};
    float deltaTime{0};
    float lastTime{0};
    int helicopter{0};

    bool fullscreen{false};
};
