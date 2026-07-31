#pragma once
#include "cevk/MeshModel.hpp"
#include "cevk/Pipeline.hpp"
#include "cevk/Textures.hpp"
#include "cevk/UBO.hpp"
#include "cevk/VulkanContext.hpp"
#include "cevk_engine/ScreenVK.hpp"
#include "cevk_infra/IStateMachine.hpp"
#include <memory>

class Game : public ce::IStateMachine {
  public:
    Game(ce::VulkanContext& context, std::shared_ptr<ce::ScreenVK> screen);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual const std::string getName() const override;

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

    ce::VulkanContext& context;
    std::shared_ptr<ce::ScreenVK> screen;

    // int currentFrame = 0;

    VkPushConstantRange pushConstantRange;

    // Scene Settings
    struct UboViewProjection {
        glm::mat4 projection;
        glm::mat4 view;
    } uboViewProjection;

    // std::shared_ptr<ce::SwapChain> swapchain;

    // std::vector<ce::CmdBuffer> cmdBuffers;

    ce::DescriptorPool descriptorPool;
    ce::UniformBuffer uniformBufferVP;
    std::shared_ptr<ce::Textures> textureMng;

    std::shared_ptr<ce::PipelineLayout> pipelineLayout;
    std::shared_ptr<ce::Pipeline> graphicPipeline;
    // std::vector<ce::Sync> syncs;
    // Scene Objects
    std::vector<ce::MeshModel> modelList;

    //
    //
    float angle{0.0F};
    float deltaTime{0};
    float lastTime{0};
    int helicopter{0};
};
