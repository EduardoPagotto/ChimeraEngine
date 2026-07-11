#include "entitys/Textures.hpp"
#include "cevk.hpp"

namespace ce {

    Textures::Textures(VkPhysicalDevice physical, VkDevice logical) : physical(physical), logical(logical) {
        //
        this->uboSampler = std::make_shared<ce::UBO<ce::ImageObject>>(logical);
        this->createDescriptorSetLayout();
        this->createDescriptorPool();
        this->createTextureSampler();
    }

    Textures::~Textures() {
        //
        vkDestroySampler(this->logical, this->textureSampler, nullptr);
        samplerDescriptorPool.reset();
        uboSampler.reset();
    }

    int Textures::createTexture(const std::string& filename, VkQueue queue, VkCommandPool commandPool) {
        // Create Texture image and get its location in array
        int textureImageLoc = this->createTextureImage(filename, queue, commandPool);
        this->uboSampler->getUBO()[textureImageLoc]->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);

        // Create Texture Descriptor
        int descritorLoc = this->createTextureDescriptor(this->uboSampler->getUBO()[textureImageLoc]->getImageView());

        // Return location of set with texture
        return descritorLoc;
    }

    void Textures::createDescriptorSetLayout() {
        //
        // CREATE TEXTURE SAMPLER DESCRIPTOR SET LAYOUT
        // Texture binding info
        this->uboSampler->addDescriptorSetLayoutBinding({.binding = 0,
                                                         .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                         .descriptorCount = 1,
                                                         .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                         .pImmutableSamplers = nullptr});

        this->uboSampler->createDescriptorSetLayout();
    }

    void Textures::createDescriptorPool() {
        // CREATE DESCRIPTOR POOL
        // -- CREATE SAMPLER DESCRIPTOR POOL
        // Texture sampler pool
        this->samplerDescriptorPool = std::make_shared<DescriptorPool>(this->logical);
        this->samplerDescriptorPool->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_OBJECTS);
        this->samplerDescriptorPool->create(MAX_OBJECTS);
    }

    void Textures::createTextureSampler() {
        // Sampler create info
        const VkSamplerCreateInfo samplerCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_LINEAR,                   // How torender when image is magnified on screen
            .minFilter = VK_FILTER_LINEAR,                   // How to render when image is minifield on screen
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,     // Mipmap interpolation mode
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // How to handle texture wrap in U(x) direction
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // How to handle texture wrap in V(y) direction
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // How to handle texture wrap in W(z) direction
            .mipLodBias = 0.0F,                              // Level of detail of bias for mip level
            .anisotropyEnable = VK_TRUE,                     // Enable anisotropy
            .maxAnisotropy = 16,                             // Anisotropy sample level
            .minLod = 0.0F,                                  // Minimum Level Detail ro pick mip level
            .maxLod = 0.0F,                                  // Maximum Level Detail ro pick mip level
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK, // Border beond texture (only works for border clamp)
            .unnormalizedCoordinates = VK_FALSE,             // Wheter coords should be normalized (between 0 and 1)
        };

        if (vkCreateSampler(logical, &samplerCreateInfo, nullptr, &this->textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Sampler");
        }
    }

    int Textures::createTextureImage(const std::string& filename, VkQueue queue, VkCommandPool commandPool) {
        // Load image
        int width;
        int height;
        VkDeviceSize imageSize;

        stbi_uc* imageData = loadTextureFile(filename, &width, &height, &imageSize);

        // Create staging buffer to hold load data, redy to copy device
        BufferObject imageStagingBuffer(this->physical, this->logical);
        imageStagingBuffer.create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // copy image data to staging buffer
        imageStagingBuffer.mapper(imageData);

        // Free original image data
        stbi_image_free(imageData);

        // create image to hold final texture
        std::shared_ptr<ce::ImageObject> texImageObj = std::make_shared<ce::ImageObject>(this->physical, this->logical);

        texImageObj->createImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // COPY DATA TO IMAGE
        // Transition image to be DST for copy operation
        TransitionImageLayout(this->logical, queue, commandPool, texImageObj->getImage(), VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy image data
        CopyImageBuffer(this->logical, queue, commandPool, imageStagingBuffer.getBuffer(), texImageObj->getImage(),
                        width, height);

        // Transition image to be shader readable for shader
        TransitionImageLayout(this->logical, queue, commandPool, texImageObj->getImage(),
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // add texture data to vector for reference
        this->uboSampler->getUBO().push_back(texImageObj);

        return this->uboSampler->getUBO().size() - 1;
    }

    int Textures::createTextureDescriptor(VkImageView textureImage) {
        //
        auto [index, size] = this->uboSampler->allocateDescriptorSets(1, this->samplerDescriptorPool->get());

        // Texture Image info
        const VkDescriptorImageInfo imageInfo{
            .sampler = this->textureSampler,                        // Image layout when in use
            .imageView = textureImage,                              // Sampler to use for set
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // Image to bind to set
        };

        // Descriptor Write info
        const VkWriteDescriptorSet descriptorWrite{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                                   .dstSet = this->uboSampler->getDescriptorSets()[index],
                                                   .dstBinding = 0,
                                                   .dstArrayElement = 0,
                                                   .descriptorCount = 1,
                                                   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                   .pImageInfo = &imageInfo};

        this->uboSampler->addWriteDescriptorSet(descriptorWrite);
        // Update new descriptor set
        this->uboSampler->updateDescriptorSets();
        this->uboSampler->clearWriteDescriptorSet();

        return this->uboSampler->getDescriptorSets().size() - 1;
    }

    stbi_uc* Textures::loadTextureFile(const std::string& filename, int* width, int* height, VkDeviceSize* imageSize) {
        // number of chanels image uses
        int channels;

        // Loads pixel data for image
        std::string fileLoc = "./assets/textures/" + filename;
        stbi_uc* image = stbi_load(fileLoc.c_str(), width, height, &channels, STBI_rgb_alpha);

        if (image == nullptr) {
            throw std::runtime_error("Failed to load a Texture file  (" + fileLoc + ") !");
        }

        // Calculate image size give a know data
        *imageSize = (*width) * (*height) * 4;

        return image;
    }

} // namespace ce
