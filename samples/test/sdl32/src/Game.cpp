#include "Game.hpp"
#include "cevk/CmdRender.hpp"
#include "cevk/Mesh.hpp"
#include "cevk/RenderPass.hpp"
#include "cevk_infra/event.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

Game::Game(std::shared_ptr<entt::registry> registry, std::shared_ptr<ce::ScreenVK> screen)
    : registry(registry), screen(screen) {

    using namespace ce;

    ctx = registry->ctx().get<std::shared_ptr<VulkanContext>>();

    this->uniformBufferVP.init(ctx->physical, ctx->logical, screen->swapchain.getSwapchainResSize(),
                               sizeof(UboViewProjection));

    this->textureMng = std::make_shared<Textures>(ctx);

    createDescriptorSetLayout();
    createPushConstantRange();
    createGraphicsPipeline();
    createDescriptorPool();
    createDescriptorSets();

    // const float radixAngle = 45.0F;
    const float near = 0.1F;
    const float far = 1000.0F;

    const float radixAngle = glm::radians(45.F); // 1:15:21
    const glm::vec3 camPos = glm::vec3(-100.0F, 150.0F, 200.0F);
    const glm::vec3 camCenter = glm::vec3(0.0F, 0.0F, -2.0F);
    const glm::vec3 camUp = glm::vec3(0.0F, 1.0F, 0.0F);
    float aspect = static_cast<float>(screen->getWidth()) / static_cast<float>(screen->getHeight());

    uboViewProjection.projection = glm::perspective(radixAngle, aspect, near, far);
    uboViewProjection.view = glm::lookAt(camPos, camCenter, camUp);
    uboViewProjection.projection[1][1] *= -1; // vulkan inverted of OpenGL

    // Create our default "no texture" texture
    std::shared_ptr<VulkanTexture> vulkanTex = VulkanTexture::Create(ctx, "./assets/textures/plain.png");
    textureMng->allocTexture(vulkanTex);

    this->inputManager = registry->ctx().get<std::shared_ptr<InputManager>>();
}

Game::~Game() {

    // Wait until no action being run on device before destroying
    vkDeviceWaitIdle(ctx->logical);

    // free(modelTransferSpace);
    for (auto& model : modelList) {
        model.destroyMeshModel();
    }

    textureMng.reset();
    descriptorPool.destroy();
    uniformBufferVP.destroy();

    graphicPipeline.reset();
    pipelineLayout.reset();
}

void Game::onAttach() {

    angle = 0.0F;
    deltaTime = 0;
    lastTime = 0;
    helicopter = this->createMeshModel("./assets/models/Seahawk.obj");
}

void Game::onDeatach() {}

void Game::onUpdate(const double& ts) {

    if (this->inputManager->keyboard->isPressed(SDLK_ESCAPE)) {
        sendChimeraEvent(ce::EventCE::FLOW_STOP, nullptr, nullptr);
    }

    if (this->inputManager->keyboard->isPressed(SDLK_F1)) {
        sendChimeraEvent(ce::EventCE::TOGGLE_FULL_SCREEN, nullptr, nullptr);
    }

    float now = SDL_GetTicks() / 1000.0F; // NOLINT
    deltaTime = now - lastTime;
    lastTime = now;

    angle += 10.0F * deltaTime;
    if (angle > 360.0F) {
        angle -= 360.0F;
    }

    glm::mat4 testMat = glm::rotate(glm::mat4(1.0F), glm::radians(angle), glm::vec3(0.0F, 1.0F, 0.0F));
    //  testMat = glm::rotate(testMat, glm::radians(-45.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    //  this->modelList[0].setModel(testMat);

    this->updateModel(helicopter, testMat);
}

bool Game::onEvent(const SDL_Event& event) {

    using namespace ce;

    switch (event.type) {
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESTORED:
            sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr);
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        case SDL_EVENT_WINDOW_MINIMIZED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr);
            break;
        default:
            return false;
    }
    return true;
}

std::string Game::getName() const { return "Game"; }

//---------------------------------------------
// Inicializaçao
//---------------------------------------------

void Game::createDescriptorSetLayout() {

    ce::DescriptorSetLayout& uniformDS = this->uniformBufferVP.getDescriptorSetLayout();

    uniformDS.addBinding({
        .binding = 0,                                        // Binding (designed by binding number in shader)
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Type of descriptor
        .descriptorCount = 1,                                // Number of descriptors for binding
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,            // Shade stage to bind to
        .pImmutableSamplers = nullptr,
    });

    uniformDS.create();
}

void Game::createPushConstantRange() {
    // Define push constant value (no 'create' needed!)
    this->pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Shader stage push constant will go to
    this->pushConstantRange.offset = 0;                              // offset into given data to pass to push constant
    this->pushConstantRange.size = sizeof(ce::Model);                // Size of data being passed
}

void Game::createGraphicsPipeline() {

    // Read in SPIR-V code shaders, Vertex Stage creation information and Fragment Stage creation information
    std::shared_ptr<ce::Shader> shader = std::make_shared<ce::Shader>(ctx->logical);

    // FIXME: trocar depois
    shader->addCode(VK_SHADER_STAGE_VERTEX_BIT, ce::aux::readFile("./bin/vert.spv"));
    shader->addCode(VK_SHADER_STAGE_FRAGMENT_BIT, ce::aux::readFile("./bin/frag.spv"));
    // shader->addCode(VK_SHADER_STAGE_VERTEX_BIT, ce::aux::readFile("./bin/bindless.vert.spv"));
    // shader->addCode(VK_SHADER_STAGE_FRAGMENT_BIT, ce::aux::readFile("./bin/bindless.frag.spv"));

    // How the data for a sigle vertex (including info such as position, colour, texture coords, normals, etc..) is as a
    // whole
    shader->addBindingDescription(0, sizeof(ce::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);

    // Attributes of shader vertex
    shader->addAtribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(ce::Vertex, pos)); // Position Attribute
    shader->addAtribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(ce::Vertex, col)); // Color Attribute
    shader->addAtribute(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(ce::Vertex, tex));    // Texture Atribute

    // -- VERTEX INPUT  ASSEMBLY INPUT --
    shader->setVertexInput(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

    // -- VIEWPORT & SCISSOR
    const VkViewport viewport{.x = 0.0F,                                         // x start coordinate
                              .y = 0.0F,                                         // y start coordinate
                              .width = static_cast<float>(screen->getWidth()),   // width of viewport
                              .height = static_cast<float>(screen->getHeight()), // height of viewport
                              .minDepth = 0.0F,                                  // min framebuffer depth
                              .maxDepth = 1.0F};                                 // max framebuffer depth

    const VkRect2D scissor{.offset = VkOffset2D{.x = 0, .y = 0}, // Offset to use region from
                           .extent =
                               screen->swapchain.getExtent()}; // Extent to describe region to use, starting at offset

    // -- PIPELINE LAYOUT --
    this->pipelineLayout = std::make_shared<ce::PipelineLayout>(this->ctx->logical);
    this->pipelineLayout->addLayout(this->uniformBufferVP.getDescriptorSetLayout().get());
    this->pipelineLayout->addLayout(this->textureMng->getUniformSampler().getDescriptorSetLayout().get());
    this->pipelineLayout->addPushRange(this->pushConstantRange);
    this->pipelineLayout->create();

    // TODO: mudar o nome da classe
    this->graphicPipeline = std::make_shared<ce::Pipeline>(this->ctx->logical);
    this->graphicPipeline->addViewport(viewport);
    this->graphicPipeline->addScissor(scissor);

    const VkPipelineColorBlendAttachmentState colourState{
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

    this->graphicPipeline->addColourState(colourState);

    // -- GRAPHICS PIPELINE CREATION
    this->graphicPipeline->create(shader, screen->renderPass.getRenderPass(), this->pipelineLayout->get());
}

void Game::createDescriptorPool() {

    // CREATE UNIFORM DESCRIPTOR POOL
    // Type of Descriptors + how many DESCRIPTORS, not Descriptor Sets (combined makes the pool size)
    // ViewProjection Pool
    this->descriptorPool.addPoolSize(
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                             .descriptorCount = static_cast<uint32_t>(this->uniformBufferVP.getBuffers().size())});

    // Create Descriptor Pool, Maximum number of descriptor Sets
    this->descriptorPool.create(this->ctx->logical, static_cast<uint32_t>(screen->swapchain.getSwapchainResSize()),
                                static_cast<VkDescriptorPoolCreateFlagBits>(0));
}

void Game::createDescriptorSets() {

    this->uniformBufferVP.allocateDescriptorSetsWithPool(this->uniformBufferVP.getBuffers().size(),
                                                         this->descriptorPool.get());

    ce::DescriptorSetWrite dsw(ctx->logical);

    // Update all of descriptor set buffer bindings
    for (size_t i = 0; i < this->uniformBufferVP.getBuffers().size(); i++) {

        ce::DescriptorSet& uboDS = this->uniformBufferVP.getDescriptorSet(i);

        // VIEW PROJECTION DESCRIPTOR
        // Buffer info and data offset info
        const VkDescriptorBufferInfo vpBufferInfo{
            .buffer = this->uniformBufferVP.getBuffers()[i]->get(), // Buffer get data from
            .offset = 0,                                            // Position of star of data
            .range = sizeof(UboViewProjection)                      // Size of data
        };

        // Data about connection between binding and buffer
        // Add to a list of descriptor set writes
        dsw.add(VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = uboDS.get(), // Descriptor Set to update
            .dstBinding = 0,       // Binding to update (matches with binding on layout/shader)
            .dstArrayElement = 0,  // index in array to update
            .descriptorCount = 1,  // type of Descriptor
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Amount to update
            .pBufferInfo = &vpBufferInfo                         // Information about buffer data to bind
        });
    }
    // Update the descripto sets with new buffer/binding info
    dsw.update();
}

//---------------------------------------------
// Loader Models
//---------------------------------------------

void Game::updateModel(size_t modelId, glm::mat4 newModel) {

    if (modelId >= this->modelList.size()) {
        return;
    }

    this->modelList[modelId].setModel(newModel);
}

size_t Game::createMeshModel(const std::string& modelFile) {
    // Import model "scene"
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(modelFile.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
                                                                    aiProcess_JoinIdenticalVertices);

    if (scene == nullptr) {
        throw std::runtime_error("Faile to load model! (" + modelFile + ")");
    }

    // Get vector of all material with 1:1 ID placement
    std::vector<std::string> textureNames = ce::MeshModel::LoadMaterials(scene);

    // Convesion from the material list IDs to our Descriptor Array IDs
    std::vector<int> matToTex(textureNames.size());

    // Loop over textureNames and create textures for them
    for (size_t i = 0; i < textureNames.size(); i++) {

        // If material had not texture, set '0' to indicate no texture, texture 0 will be reserved for a default texture
        if (textureNames[i].empty()) { // FIXME: talvez set seria melhor
            matToTex[i] = 0;
        } else {

            // Otherwise, create texture and set value to index of new texture
            std::shared_ptr<ce::VulkanTexture> vulkanTex =
                ce::VulkanTexture::Create(ctx, "./assets/textures/" + textureNames[i]);

            matToTex[i] = static_cast<int>(textureMng->allocTexture(vulkanTex));
        }
    }

    // Load in all our meshes
    std::vector<ce::Mesh> modelMeshes = ce::MeshModel::LoadNode(ctx->physical, ctx->logical, ctx->graphicsQueue,
                                                                ctx->commandPool, scene->mRootNode, scene, matToTex);

    // Create mesh model and add to list
    ce::MeshModel meshModel(modelMeshes);
    this->modelList.push_back(meshModel);

    return this->modelList.size() - 1;
}

void Game::onRender() {

    // -- GET NEXT IMAGE --
    ce::Frame& frame = screen->frames[screen->currentFrame];

    // Get index of next image to be draw to, execute sincronization
    auto [imageIndex, swapchainRes] =
        screen->swapchain.acquireNextImage(frame.inFlightFence, frame.imageAvailableSemaphore);

    VkRenderPassBeginInfo renderPassBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = screen->renderPass.getRenderPass(),                     // Render pass to begin
        .framebuffer = swapchainRes.framebuffer,                              //
        .renderArea = screen->swapchain.getRenderArea(),                      //
        .clearValueCount = static_cast<uint32_t>(screen->clearValues.size()), //
        .pClearValues = screen->clearValues.data(),                           // List of clear values
    };

    ce::CmdRender cmd;
    cmd.begin(frame.commandBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, renderPassBeginInfo,
              this->graphicPipeline->get());

    // Copy View Projection data in UBO
    this->uniformBufferVP.getBuffers()[imageIndex]->mapper(&this->uboViewProjection);

    ce::DescriptorSet& vpUboDS = this->uniformBufferVP.getDescriptorSet(imageIndex);

    for (size_t j = 0; j < this->modelList.size(); j++) {

        ce::MeshModel thisModel = modelList[j];

        cmd.pushConstants(this->pipelineLayout->get(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ce::Model),
                          &thisModel.getModel2());

        for (size_t k = 0; k < thisModel.getMeshCount(); k++) {

            ce::DescriptorSet& samplerUboDS =
                this->textureMng->getUniformSampler().getDescriptorSet(thisModel.getMesh(k)->getTexId());

            cmd.addVertexBuffer({0}, thisModel.getMesh(k)->getVertexBuffer());
            cmd.bindVertexBuffer(0);
            cmd.bindIndexBuffer(thisModel.getMesh(k)->getIndexBuffer(), {0});
            cmd.addDescriptorSet(vpUboDS.get());
            cmd.addDescriptorSet(samplerUboDS.get());
            cmd.bindDescriptorSets(this->pipelineLayout->get());
            cmd.drawIndexed(thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
            cmd.clearTemps();
        }
    }

    cmd.end();

    // -- SUBMIT COMMAND BUFFER TO RENDER
    cmd.submitToRender(ctx->graphicsQueue, &frame, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    // -- PRESENT RENDERED IMAGE TO SCREEN --
    VkResult result = ce::RenderPass::SendImageToScreen(ctx->presentationQueue, frame.renderFinishedSemaphore,
                                                        screen->swapchain.getSwapchain(), imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) { //|| framebufferResized
        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "resized (%d)...", result);
        // framebufferResized = false;
        screen->swapchain.recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present Swapchain!");
    }
}
