#include "VulkanRenderer.hpp"
#include "Command.hpp"
#include <cstddef>
#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <stdexcept>

VulkanRenderer::VulkanRenderer(ce::DevVk& devvk) {

    using namespace ce;

    this->bvk = devvk.getBaseVK();
    this->gQueue = devvk.getGraphicsQueue();
    this->pQueue = devvk.getPresentationQueue();

    this->swapchain = std::make_shared<SwapChain>(bvk.get());
    this->rederer = std::make_shared<Renderer>(bvk.get(), swapchain->getImageFormat());
    this->rederer->setRederArea({.offset = {.x = 0, .y = 0}, .extent = this->swapchain->getExtent()});

    this->uboVP = std::make_shared<UBO<Buffer>>(bvk->physical, bvk->logical, swapchain->getImages().size(),
                                                sizeof(UboViewProjection));
    this->textureMng = std::make_shared<Textures>(bvk->physical, bvk->logical);

    createDescriptorSetLayout();
    createPushConstantRange();
    createGraphicsPipeline();

    swapchain->createFramebuffers(rederer->getRenderPass());

    graphicsCommandPool = std::make_shared<CommandPool>(this->bvk.get());

    cmdBuffers.resize(swapchain->getSwapChainFrameBuffers().size());
    for (size_t i = 0; i < swapchain->getSwapChainFrameBuffers().size(); i++) {
        cmdBuffers[i] = CommandBuffer();
        cmdBuffers[i].init(bvk->logical, graphicsCommandPool->get());
    }

    createDescriptorPool();
    createDescriptorSets();

    this->syncs.resize(ce::MAX_FRAME_DRAWS);
    for (size_t i = 0; i < ce::MAX_FRAME_DRAWS; i++) {
        this->syncs[i] = ce::Sync();
        this->syncs[i].init(this->bvk->logical);
    }

    // const float radixAngle = 45.0F;
    const float near = 0.1F;
    const float far = 1000.0F;

    const float radixAngle = glm::radians(45.F); // 1:15:21
    const glm::vec3 camPos = glm::vec3(-100.0F, 150.0F, 200.0F);
    const glm::vec3 camCenter = glm::vec3(0.0F, 0.0F, -2.0F);
    const glm::vec3 camUp = glm::vec3(0.0F, 1.0F, 0.0F);
    const float aspect = (float)swapchain->getExtent().width / (float)swapchain->getExtent().height;

    uboViewProjection.projection = glm::perspective(radixAngle, aspect, near, far);
    uboViewProjection.view = glm::lookAt(camPos, camCenter, camUp);
    uboViewProjection.projection[1][1] *= -1; // vulkan inverted of OpenGL

    // Create our default "no texture" texture
    textureMng->createTexture("plain.png", gQueue, graphicsCommandPool->get());
}

VulkanRenderer::~VulkanRenderer() {

    // Wait until no action being run on device before destroying
    vkDeviceWaitIdle(bvk->logical);

    // free(modelTransferSpace);
    for (auto& model : modelList) {
        model.destroyMeshModel();
    }

    textureMng.reset();
    descriptorPool.reset();
    uboVP.reset();

    for (size_t i = 0; i < this->syncs.size(); i++) {
        this->syncs[i].destroy();
    }

    for (size_t i = 0; i < cmdBuffers.size(); i++) {
        cmdBuffers[i].destroy();
    }

    graphicsCommandPool.reset();
    graphicPipeline.reset();
    pipelineLayout.reset();
}

void VulkanRenderer::updateModel(int modelId, glm::mat4 newModel) {

    if (modelId >= this->modelList.size()) {
        return;
    }

    this->modelList[modelId].setModel(newModel);
}

void VulkanRenderer::draw() {
    // // -- GET NEXT IMAGE --
    // // Wait for given fence to signal (open) from last draw before continuing
    // // Manually reset (close) fence
    auto& sync = this->syncs[this->currentFrame];
    sync.waitAndResetFence();

    // Get index of next image to be draw to, and signal semaphore when ready to be draw to
    uint32_t imageIndex;
    vkAcquireNextImageKHR(bvk->logical, this->swapchain->getKHR(), std::numeric_limits<uint64_t>::max(), sync.getWait(),
                          VK_NULL_HANDLE, &imageIndex);

    this->recordCommands(imageIndex);
    // Copy View Projection data in UBO
    this->uboVP->getUBO()[imageIndex]->mapper(&this->uboViewProjection);

    // -- SUBMIT COMMAND BUFFER TO RENDER
    this->cmdBuffers[imageIndex].submitToRender(gQueue, sync, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    // -- PRESENT RENDERED IMAGE TO SCREEN --
    this->swapchain->sendImageToScreen(pQueue, sync.getSignal(), imageIndex);
    // Get next frame
    this->currentFrame = (this->currentFrame + 1) % ce::MAX_FRAME_DRAWS;
    // AHHHH!!!!!! ugly!!!!! this is complete wrong, find what missmatch sYncs!!!
    if (this->currentFrame == (ce::MAX_FRAME_DRAWS - 1)) {
        vkDeviceWaitIdle(bvk->logical);
    }
}

void VulkanRenderer::createDescriptorSetLayout() {

    // UNIFORM VALUES DESCRIPTOR SET LAYOUT
    // UboViewProjection Binding info
    this->uboVP->addDescriptorSetLayoutBinding({
        .binding = 0, // Binding point in shader (designed by binding number in shader)
        .descriptorType =
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,    // Type of descriptor (uniform, dynamic, image sampler, etc)
        .descriptorCount = 1,                     // Number of descriptors for binding
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT, // Shade stage to bind to
        .pImmutableSamplers = nullptr, // for Texture: can make sampler unchangeable (immutable) by specifying in layout
    });

    // // Model Binding Info
    // this->uboVP->addDescriptorSetLayoutBinding({.binding = 1,
    //                                             .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    //                                             .descriptorCount = 1,
    //                                             .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    //                                             .pImmutableSamplers = nullptr});

    this->uboVP->createDescriptorSetLayout();
}

void VulkanRenderer::createPushConstantRange() {
    // Define push constant value (no 'create' needed!)
    this->pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Shader stage push constant will go to
    this->pushConstantRange.offset = 0;                              // offset into given data to pass to push constant
    this->pushConstantRange.size = sizeof(ce::Model);                // Size of data being passed
}

void VulkanRenderer::createGraphicsPipeline() {

    // Read in SPIR-V code shaders, Vertex Stage creation information and Fragment Stage creation information
    std::shared_ptr<ce::Shader> shader = std::make_shared<ce::Shader>(bvk->logical);
    shader->addCode(VK_SHADER_STAGE_VERTEX_BIT, ce::aux::readFile("./bin/vert.spv"));
    shader->addCode(VK_SHADER_STAGE_FRAGMENT_BIT, ce::aux::readFile("./bin/frag.spv"));

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
    const VkViewport viewport{.x = 0.0F,                                            // x start coordinate
                              .y = 0.0F,                                            // y start coordinate
                              .width = (float)this->swapchain->getExtent().width,   // width of viewport
                              .height = (float)this->swapchain->getExtent().height, // height of viewport
                              .minDepth = 0.0F,                                     // min framebuffer depth
                              .maxDepth = 1.0F};                                    // max framebuffer depth

    const VkRect2D scissor{.offset = VkOffset2D{.x = 0, .y = 0}, // Offset to use region from
                           .extent =
                               this->swapchain->getExtent()}; // Extent to describe region to use, starting at offset

    // -- PIPELINE LAYOUT --
    this->pipelineLayout = std::make_shared<ce::PipelineLayout>(this->bvk->logical);
    this->pipelineLayout->addLayout(this->uboVP->getDescriptorSetLayout());
    this->pipelineLayout->addLayout(this->textureMng->getUbo()->getDescriptorSetLayout());
    this->pipelineLayout->addPushRange(this->pushConstantRange);
    this->pipelineLayout->create();

    // TODO: mudar o nome da classe
    this->graphicPipeline = std::make_shared<ce::Pipeline>(this->bvk->logical);
    this->graphicPipeline->addViewport(viewport);
    this->graphicPipeline->addScissor(scissor);

    // // -- DYNAMIC STATES --
    // this->graphicPipeline->addDynamicStateEnables(VK_DYNAMIC_STATE_VIEWPORT); // Dynamic Viewport: Can resize in
    // command buffer with ;                                                                        //
    // vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
    // this->graphicPipeline->addDynamicStateEnables(VK_DYNAMIC_STATE_SCISSOR);  // Dynamic Scissor: Can resize in
    // command buffer with ;                                                                        //
    // vkCmdSetViewport(commandbuffer, 0, 1, &scissor);

    // Blend Attachment State (how blending is handled)
    // Blending uses equation: (srcColorBlendfactor * new colour) colorBlendOp (dstColorBlendfactor * old colour)
    // Sumarised 1: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour)
    //            (new colour alpha * new colour) + ((i - new color alpha) * old colour)
    // Sumarized 2: (1 * new alpha) + (0 * old Alpha) = new alpha

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
    this->graphicPipeline->create(shader, this->rederer->getRenderPass(), this->pipelineLayout->get());
}

void VulkanRenderer::createDescriptorPool() {

    // CREATE DESCRIPTOR POOL
    // CREATE UNIFORM DESCRIPTOR POOL
    this->descriptorPool = std::make_shared<ce::DescriptorPool>(this->bvk->logical);
    // Type of Descriptors + how many DESCRIPTORS, not Descriptor Sets (combined makes the pool size)
    // ViewProjection Pool
    this->descriptorPool->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(this->uboVP->size()));

    // // Model Pool (Dynamic)
    // this->descriptorPool->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, //
    //                                    static_cast<uint32_t>(this->modelDUniformBuffer.size());//

    // Create Descriptor Pool
    this->descriptorPool->create(
        static_cast<uint32_t>(this->swapchain->getImages().size())); // Maximum number of descriptor Sets
}

void VulkanRenderer::createDescriptorSets() {

    this->uboVP->allocateDescriptorSets(this->uboVP->size(), this->descriptorPool->get());

    // Update all of descriptor set buffer bindings
    for (size_t i = 0; i < this->swapchain->getImages().size(); i++) {
        // VIEW PROJECTION DESCRIPTOR
        // Buffer info and data offset info
        const VkDescriptorBufferInfo vpBufferInfo{
            .buffer = this->uboVP->getUBO()[i]->getBuffer(), // Buffer get data from
            .offset = 0,                                     // Position of star of data
            .range = sizeof(UboViewProjection)               // Size of data
        };

        // Data about connection between binding and buffer
        const VkWriteDescriptorSet vpSetWrite{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet =
                this->uboVP
                    ->getDescriptorSets()[i], // this->descriptorSets->get()[i],            // Descriptor Set to update
            .dstBinding = 0,                  // Binding to update (matches with binding on layout/shader)
            .dstArrayElement = 0,             // index in array to update
            .descriptorCount = 1,             // type of Descriptor
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Amount to update
            .pBufferInfo = &vpBufferInfo                         // Information about buffer data to bind
        };

        // MODEL DESCRIPTOR
        // Model buffer binding info
        // VkDescriptorBufferInfo modelBufferInfo {
        //     .buffer = this->modelDUniformBuffer[i],
        //     .offset = 0,
        //     .range = this->modelUniformAlignment
        // };

        // // Data about connection between binding and buffer
        // VkWriteDescriptorSet modelSetWrite {
        //     .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        //     .dstSet = this->descriptorSets[i],
        //     .dstBinding = 1,
        //     .dstArrayElement = 0,
        //     .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        //     .descriptorCount = 1,
        //     .pBufferInfo = &modelBufferInfo
        // };

        // Add to a list of descriptor set writes
        this->uboVP->addWriteDescriptorSet(vpSetWrite);
    }
    // Update the descripto sets with new buffer/binding info
    this->uboVP->updateDescriptorSets();
    this->uboVP->clearWriteDescriptorSet();
}

void VulkanRenderer::recordCommands(uint32_t currentImage) {

    VkRenderPassBeginInfo renderPassBeginInfo{};
    this->rederer->passBegin(this->swapchain->getSwapChainFrameBuffers()[currentImage], &renderPassBeginInfo);

    ce::Command cmd(this->cmdBuffers[currentImage].get(), VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    cmd.beginAndPipeline(renderPassBeginInfo, this->graphicPipeline->get());

    for (size_t j = 0; j < this->modelList.size(); j++) {

        ce::MeshModel thisModel = modelList[j];

        cmd.pushConstants(this->pipelineLayout->get(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ce::Model),
                          &thisModel.getModel2());

        for (size_t k = 0; k < thisModel.getMeshCount(); k++) {

            cmd.addVertexBuffer({0}, thisModel.getMesh(k)->getVertexBuffer());
            cmd.bindVertexBuffer(0);
            cmd.bindIndexBuffer(thisModel.getMesh(k)->getIndexBuffer(), {0});
            cmd.addDescriptorSet(this->uboVP->getDescriptorSets()[currentImage]);
            cmd.addDescriptorSet(this->textureMng->getUbo()->getDescriptorSets()[thisModel.getMesh(k)->getTexId()]);
            cmd.bindDescriptorSets(this->pipelineLayout->get());
            cmd.drawIndexed(thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
            cmd.clearTemps();
        }
    }

    cmd.end();
}

int VulkanRenderer::createMeshModel(const std::string& modelFile) {
    // Import model "scene"
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(modelFile.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
                                                                    aiProcess_JoinIdenticalVertices);

    if (scene == nullptr) {
        throw std::runtime_error("Faile to load model! (" + modelFile + ")");
    }

    // Get vector of all material with 1:1 ID placement
    std::vector<std::string> textureNames = ce::MeshModel::loadMaterials(scene);

    // Convesion from the material list IDs to our Descriptor Array IDs
    std::vector<int> matToTex(textureNames.size());

    // Loop over textureNames and create textures for them
    for (size_t i = 0; i < textureNames.size(); i++) {

        // If material had not texture, set '0' to indicate no texture, texture 0 will be reserved for a default texture
        if (textureNames[i].empty()) { // FIXME: talvez set seria melhor
            matToTex[i] = 0;
        } else {

            // Otherwise, create texture and set value to index of new texture
            matToTex[i] =
                this->textureMng->createTexture(textureNames[i], this->gQueue, this->graphicsCommandPool->get());
            // matToTex[i] = createTexture("panda.jpg");
        }
    }

    // Load in all our meshes
    std::vector<ce::Mesh> modelMeshes = ce::MeshModel::LoadNode(
        bvk->physical, bvk->logical, gQueue, this->graphicsCommandPool->get(), scene->mRootNode, scene, matToTex);

    // Create mesh model and add to list
    ce::MeshModel meshModel(modelMeshes);
    this->modelList.push_back(meshModel);

    return this->modelList.size() - 1;
}
