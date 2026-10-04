#include "Game.hpp"
#include "cevk/CmdRender.hpp"
#include "cevk/Mesh.hpp"
#include "chimera_base/event.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

Game::Game(std::shared_ptr<entt::registry> registry) : registry_(registry) {

    using namespace ce;

    canvas_ = std::dynamic_pointer_cast<ce::CanvaVK>(registry->ctx().get<std::shared_ptr<ce::ICanva>>());
    ctx_ = canvas_->ctx();

    this->uniform_buffer_vp_.init(ctx_->physical, ctx_->logical, canvas_->swapchain.get_swapchain_res_size(),
                                  sizeof(UboViewProjection));

    this->texture_mng_ = std::make_shared<Textures>(ctx_);

    create_descriptorset_layout();
    create_pushconstant_range();
    create_graphics_pipeline();
    create_descriptorpool();
    create_descriptorsets();

    // const float radixAngle = 45.0F;
    const float near = 0.1F;
    const float far = 1000.0F;

    const float radix_angle = glm::radians(45.F); // 1:15:21
    const glm::vec3 cam_pos = glm::vec3(-100.0F, 150.0F, 200.0F);
    const glm::vec3 cam_center = glm::vec3(0.0F, 0.0F, -2.0F);
    const glm::vec3 cam_up = glm::vec3(0.0F, 1.0F, 0.0F);
    float aspect = static_cast<float>(canvas_->width()) / static_cast<float>(canvas_->height());

    ubo_view_projection_.projection = glm::perspective(radix_angle, aspect, near, far);
    ubo_view_projection_.view = glm::lookAt(cam_pos, cam_center, cam_up);
    ubo_view_projection_.projection[1][1] *= -1; // vulkan inverted of OpenGL

    // Create our default "no texture" texture
    std::shared_ptr<VulkanTexture> vulkan_tex = VulkanTexture::create(ctx_, "./assets/textures/plain.png");
    texture_mng_->alloc_texture(vulkan_tex);

    this->input_manager_ = registry->ctx().get<std::shared_ptr<InputManager>>();
}

Game::~Game() {

    // Wait until no action being run on device before destroying
    vkDeviceWaitIdle(ctx_->logical);

    // free(modelTransferSpace);
    for (auto& model : model_list_) {
        model.destroyMeshModel();
    }

    texture_mng_.reset();
    descriptor_pool_.destroy();
    uniform_buffer_vp_.destroy();

    graphic_pipeline_.reset();
    pipeline_layout_.reset();
}

void Game::on_attach() {

    angle_ = 0.0F;
    delta_time_ = 0;
    last_time_ = 0;
    helicopter_ = this->create_mesh_model("./assets/models/Seahawk.obj");
}

void Game::on_deatach() {}

void Game::on_update(const double& ts) {

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
        return;
    }

    if (this->input_manager_->get_keyboard()->is_key_pressed(SDL_SCANCODE_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
        return;
    }

    float now = SDL_GetTicks() / 1000.0F; // NOLINT
    delta_time_ = now - last_time_;
    last_time_ = now;

    angle_ += 10.0F * delta_time_;
    if (angle_ > 360.0F) {
        angle_ -= 360.0F;
    }

    glm::mat4 test_mat = glm::rotate(glm::mat4(1.0F), glm::radians(angle_), glm::vec3(0.0F, 1.0F, 0.0F));
    //  testMat = glm::rotate(testMat, glm::radians(-45.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    //  this->modelList[0].setModel(testMat);

    if (canvas_->eventReShape) {
        canvas_->eventReShape = false;
        float aspect = static_cast<float>(canvas_->width()) / static_cast<float>(canvas_->height());

        const float near = 0.1F;
        const float far = 1000.0F;
        const float radix_angle = glm::radians(45.F); // 1:15:21
        ubo_view_projection_.projection = glm::perspective(radix_angle, aspect, near, far);
        ubo_view_projection_.projection[1][1] *= -1; // vulkan inverted of OpenGL
    }

    this->update_model(helicopter_, test_mat);
}

void Game::on_event(const SDL_Event& event) {}

std::string Game::get_name() const { return "Game"; }

//---------------------------------------------
// Inicializaçao
//---------------------------------------------

void Game::create_descriptorset_layout() {

    ce::DescriptorSetLayout& uniform_ds = this->uniform_buffer_vp_.get_descriptor_set_layout();

    uniform_ds.add_binding({
        .binding = 0,                                        // Binding (designed by binding number in shader)
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Type of descriptor
        .descriptorCount = 1,                                // Number of descriptors for binding
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,            // Shade stage to bind to
        .pImmutableSamplers = nullptr,
    });

    uniform_ds.create();
}

void Game::create_pushconstant_range() {
    // Define push constant value (no 'create' needed!)
    this->push_constant_range_.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Shader stage push constant will go to
    this->push_constant_range_.offset = 0;               // offset into given data to pass to push constant
    this->push_constant_range_.size = sizeof(ce::Model); // Size of data being passed
}

void Game::create_graphics_pipeline() {

    // Read in SPIR-V code shaders, Vertex Stage creation information and Fragment Stage creation information
    std::shared_ptr<ce::Shader> shader = std::make_shared<ce::Shader>(ctx_->logical);
    shader->add_code(VK_SHADER_STAGE_VERTEX_BIT, ce::aux::readFile("./bin/vert.spv"));
    shader->add_code(VK_SHADER_STAGE_FRAGMENT_BIT, ce::aux::readFile("./bin/frag.spv"));

    // How the data for a sigle vertex (including info such as position, colour, texture coords, normals, etc..) is as a
    // whole
    shader->add_binding_description(0, sizeof(ce::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);

    // Attributes of shader vertex
    shader->add_atribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(ce::Vertex, pos)); // Position Attribute
    shader->add_atribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(ce::Vertex, col)); // Color Attribute
    shader->add_atribute(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(ce::Vertex, tex));    // Texture Atribute

    // -- VERTEX INPUT  ASSEMBLY INPUT --
    shader->set_vertex_input(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

    // -- VIEWPORT & SCISSOR
    const VkViewport viewport{.x = 0.0F,                                       // x start coordinate
                              .y = 0.0F,                                       // y start coordinate
                              .width = static_cast<float>(canvas_->width()),   // width of viewport
                              .height = static_cast<float>(canvas_->height()), // height of viewport
                              .minDepth = 0.0F,                                // min framebuffer depth
                              .maxDepth = 1.0F};                               // max framebuffer depth

    const VkRect2D scissor{.offset = VkOffset2D{.x = 0, .y = 0}, // Offset to use region from
                           .extent =
                               canvas_->swapchain.get_extent()}; // Extent to describe region to use, starting at offset

    // -- PIPELINE LAYOUT --
    this->pipeline_layout_ = std::make_shared<ce::PipelineLayout>(this->ctx_->logical);
    this->pipeline_layout_->add_layout(this->uniform_buffer_vp_.get_descriptor_set_layout().get());
    this->pipeline_layout_->add_layout(this->texture_mng_->get_uniform_sampler().get_descriptor_set_layout().get());
    this->pipeline_layout_->add_push_range(this->push_constant_range_);
    this->pipeline_layout_->create();

    // TODO: mudar o nome da classe
    this->graphic_pipeline_ = std::make_shared<ce::Pipeline>(this->ctx_->logical);
    this->graphic_pipeline_->add_viewport(viewport);
    this->graphic_pipeline_->add_scissor(scissor);

    const VkPipelineColorBlendAttachmentState colour_state{
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                          VK_COLOR_COMPONENT_A_BIT, // Color to apply blending to
    };

    this->graphic_pipeline_->add_colour_state(colour_state);

    // -- GRAPHICS PIPELINE CREATION
    this->graphic_pipeline_->create(shader, canvas_->renderPass.get_render_pass(), this->pipeline_layout_->get());
}

void Game::create_descriptorpool() {

    // CREATE UNIFORM DESCRIPTOR POOL
    // Type of Descriptors + how many DESCRIPTORS, not Descriptor Sets (combined makes the pool size)
    // ViewProjection Pool
    this->descriptor_pool_.add_pool_size(
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                             .descriptorCount = static_cast<uint32_t>(this->uniform_buffer_vp_.get_buffers().size())});

    // Create Descriptor Pool, Maximum number of descriptor Sets
    this->descriptor_pool_.create(this->ctx_->logical,
                                  static_cast<uint32_t>(canvas_->swapchain.get_swapchain_res_size()),
                                  static_cast<VkDescriptorPoolCreateFlagBits>(0));
}

void Game::create_descriptorsets() {

    this->uniform_buffer_vp_.allocate_descriptor_sets_with_pool(this->uniform_buffer_vp_.get_buffers().size(),
                                                                this->descriptor_pool_.get());

    ce::DescriptorSetWrite dsw(ctx_->logical);

    // Update all of descriptor set buffer bindings
    for (size_t i = 0; i < this->uniform_buffer_vp_.get_buffers().size(); i++) {

        ce::DescriptorSet& ubo_ds = this->uniform_buffer_vp_.get_descriptor_set(i);

        // VIEW PROJECTION DESCRIPTOR
        // Buffer info and data offset info
        const VkDescriptorBufferInfo vp_buffer_info{
            .buffer = this->uniform_buffer_vp_.get_buffers()[i]->get(), // Buffer get data from
            .offset = 0,                                                // Position of star of data
            .range = sizeof(UboViewProjection)                          // Size of data
        };

        // Data about connection between binding and buffer
        // Add to a list of descriptor set writes
        dsw.add(VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = ubo_ds.get(), // Descriptor Set to update
            .dstBinding = 0,        // Binding to update (matches with binding on layout/shader)
            .dstArrayElement = 0,   // index in array to update
            .descriptorCount = 1,   // type of Descriptor
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Amount to update
            .pBufferInfo = &vp_buffer_info                       // Information about buffer data to bind
        });
    }
    // Update the descripto sets with new buffer/binding info
    dsw.update();
}

//---------------------------------------------
// Loader Models
//---------------------------------------------

void Game::update_model(size_t model_id, glm::mat4 new_model) {

    if (model_id >= this->model_list_.size()) {
        return;
    }

    this->model_list_[model_id].setModel(new_model);
}

size_t Game::create_mesh_model(const std::string& model_file) {
    // Import model "scene"
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(model_file.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
                                                                     aiProcess_JoinIdenticalVertices);

    if (scene == nullptr) {
        throw std::runtime_error("Faile to load model! (" + model_file + ")");
    }

    // Get vector of all material with 1:1 ID placement
    std::vector<std::string> texture_names = ce::MeshModel::LoadMaterials(scene);

    // Convesion from the material list IDs to our Descriptor Array IDs
    std::vector<int> mat_to_tex(texture_names.size());

    // Loop over textureNames and create textures for them
    for (size_t i = 0; i < texture_names.size(); i++) {

        // If material had not texture, set '0' to indicate no texture, texture 0 will be reserved for a default texture
        if (texture_names[i].empty()) { // FIXME: talvez set seria melhor
            mat_to_tex[i] = 0;
        } else {

            // Otherwise, create texture and set value to index of new texture
            std::shared_ptr<ce::VulkanTexture> vulkan_tex =
                ce::VulkanTexture::create(ctx_, "./assets/textures/" + texture_names[i]);

            mat_to_tex[i] = static_cast<int>(texture_mng_->alloc_texture(vulkan_tex));
        }
    }

    // Load in all our meshes
    std::vector<ce::Mesh> model_meshes = ce::MeshModel::LoadNode(
        ctx_->physical, ctx_->logical, ctx_->graphicsQueue, ctx_->commandPool, scene->mRootNode, scene, mat_to_tex);

    // Create mesh model and add to list
    ce::MeshModel mesh_model(model_meshes);
    this->model_list_.push_back(mesh_model);

    return this->model_list_.size() - 1;
}

//---------------------------------------------
// Loader Models
//---------------------------------------------

void Game::on_render() {

    // -- GET NEXT IMAGE --
    ce::Frame& frame = canvas_->frames[canvas_->currentFrame];

    // Get index of next image to be draw to, execute sincronization
    auto [imageIndex, renderPassBeginInfo] = canvas_->next_image_renderpass();

    ce::CmdRender cmd;
    cmd.begin(frame.commandBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, renderPassBeginInfo,
              this->graphic_pipeline_->get());

    // Copy View Projection data in UBO
    this->uniform_buffer_vp_.get_buffers()[imageIndex]->mapper(&this->ubo_view_projection_);

    ce::DescriptorSet& vp_ubo_ds = this->uniform_buffer_vp_.get_descriptor_set(imageIndex);

    for (size_t j = 0; j < this->model_list_.size(); j++) {

        ce::MeshModel this_model = model_list_[j];

        cmd.push_constants(this->pipeline_layout_->get(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ce::Model),
                           &this_model.getModel2());

        for (size_t k = 0; k < this_model.getMeshCount(); k++) {

            ce::DescriptorSet& sampler_ubo_ds =
                this->texture_mng_->get_uniform_sampler().get_descriptor_set(this_model.getMesh(k)->get_tex_id());

            cmd.add_vertex_buffer({0}, this_model.getMesh(k)->get_vertex_buffer());
            cmd.bind_vertex_buffer(0);
            cmd.bind_index_buffer({0}, this_model.getMesh(k)->get_index_buffer());
            cmd.add_descriptor_set(vp_ubo_ds.get());
            cmd.add_descriptor_set(sampler_ubo_ds.get());
            cmd.bind_descriptor_sets(this->pipeline_layout_->get());
            cmd.draw_indexed(this_model.getMesh(k)->get_index_count(), 1, 0, 0, 0);
            cmd.clear_temps();
        }
    }

    cmd.end();

    // -- SUBMIT COMMAND BUFFER TO RENDER
    cmd.submit_to_render(ctx_->graphicsQueue, &frame, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
}
