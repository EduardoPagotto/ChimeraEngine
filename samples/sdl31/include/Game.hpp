#pragma once
#include "MeshModel.hpp"
#include "cevk/Pipeline.hpp"
#include "cevk/Textures.hpp"
#include "cevk/UBO.hpp"
#include "cevk/VulkanContext.hpp"
#include "cevk_engine/CanvaVK.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include <entt/entt.hpp>
#include <memory>

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<entt::registry> registry);
    virtual ~Game();
    virtual void on_attach() override;
    virtual void on_deatach() override;
    virtual void on_render() override;
    virtual void on_update(const double& ts) override;
    virtual void on_event(const SDL_Event& event) override;
    virtual std::string get_name() const override;

  private:
    // - Vulkan create functions
    void create_descriptorset_layout();
    void create_pushconstant_range();
    void create_graphics_pipeline();
    void create_descriptorpool();
    void create_descriptorsets();

    void update_model(size_t model_id, glm::mat4 new_model);
    size_t create_mesh_model(const std::string& model_file);

    std::shared_ptr<ce::CanvaVK> canvas_;
    std::shared_ptr<ce::VulkanContext> ctx_;

    VkPushConstantRange push_constant_range_;

    // Scene Settings
    struct UboViewProjection {
        glm::mat4 projection;
        glm::mat4 view;
    } ubo_view_projection_;

    ce::DescriptorPool descriptor_pool_;
    ce::UniformBuffer uniform_buffer_vp_;

    std::shared_ptr<ce::Textures> texture_mng_;
    std::shared_ptr<ce::PipelineLayout> pipeline_layout_;
    std::shared_ptr<ce::Pipeline> graphic_pipeline_;

    // Scene Objects
    std::vector<ce::MeshModel> model_list_;
    //
    float angle_{0.0F};
    float delta_time_{0};
    float last_time_{0};
    size_t helicopter_{0};

    std::shared_ptr<entt::registry> registry_;

    std::shared_ptr<ce::InputManager> input_manager_;
};
