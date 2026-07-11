#ifndef __BUFFERS_CLASS_HPP
#define __BUFFERS_CLASS_HPP

#include "Descriptors.hpp"
#include "cevk.hpp"
#include <memory>

namespace ce {

#pragma region BufferObjec

    class BufferObject {
      public:
        explicit BufferObject(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~BufferObject();

        // Proíbe cópia para evitar dupla desalocação
        BufferObject(const BufferObject&) = delete;
        BufferObject& operator=(const BufferObject&) = delete;
        // BufferObject(BufferObject&& other) noexcept;
        // BufferObject& operator=(BufferObject&& other) noexcept;

        void create(const VkDeviceSize& bufferSize, const VkBufferUsageFlags& bufferUsage,
                    const VkMemoryPropertyFlags& bufferProperties);

        [[nodiscard]] VkBuffer getBuffer() const { return buffer; }
        [[nodiscard]] VkDeviceMemory getMemory() const { return memory; }
        [[nodiscard]] bool isValid() const { return memory != VK_NULL_HANDLE; }
        //[[nodiscard]] void* getMappedData() const { return mappedData; }

        void mapper(void* src);

      private:
        void destroy();

        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkBuffer buffer{VK_NULL_HANDLE};
        VkDeviceMemory memory{VK_NULL_HANDLE};
        VkDeviceSize bufferSize;
        // void* mappedData{nullptr};
    };

#pragma endregion

#pragma region VBO

    class VBO {
      public:
        explicit VBO(VkPhysicalDevice physical, VkDevice logical);
        virtual ~VBO();

        VBO(const VBO&) = delete;
        VBO& operator=(const VBO&) = delete;
        // VBO(VBO&& other) noexcept;
        // VBO& operator=(VBO&& other) noexcept;

        [[nodiscard]] size_t getCount() const { return this->count; }
        [[nodiscard]] VkBuffer getBuffer() const { return this->vbo->getBuffer(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool commandPool, std::vector<Vertex>* vertices, size_t sizeVertex);

      private:
        size_t count;
        VkPhysicalDevice physical;
        VkDevice logical;
        std::shared_ptr<ce::BufferObject> vbo;
    };

#pragma endregion

#pragma region ImageObject

    class ImageObject {
      public:
        explicit ImageObject(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~ImageObject();

        ImageObject(const ImageObject&) = delete;
        ImageObject& operator=(const ImageObject&) = delete;

        void createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags);

        void createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void createImageView(VkImageAspectFlags aspectFlags);

        VkImageView& getImageView() { return this->imageView; }
        VkImage& getImage() { return this->image; }
        VkDeviceMemory& getImageMemory() { return this->imageMemory; }

      private:
        void destroy();

        bool isImported{false};
        VkFormat format;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkImageView imageView{VK_NULL_HANDLE};
        VkImage image{VK_NULL_HANDLE};
        VkDeviceMemory imageMemory{VK_NULL_HANDLE};
    };

#pragma endregion

#pragma region CommandBuffer

    struct SubmitToRenderInfo {
        VkQueue gQueue;
        VkSemaphore wait;
        VkSemaphore signal;
        VkFence fence;
        VkPipelineStageFlagBits pipelineStageFlags;
        size_t bufferIndex;
    };

    class CommandBuffer {
      public:
        explicit CommandBuffer(VkDevice device, VkCommandPool commandPool, size_t count);
        virtual ~CommandBuffer();

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;

        void clean(size_t index);
        void cleanAll();
        void begin(size_t index, VkCommandBufferUsageFlagBits flag);
        void end(size_t index);

        std::vector<VkCommandBuffer>& getBuffers() { return this->commandBuffers; }

        void submitToRender(const SubmitToRenderInfo& sub);

      private:
        VkDevice device;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };

    namespace aux {

        void SubmitQueue(VkQueue queue, VkCommandBuffer commandBuffer);

        void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                        VkBuffer dstBuffer, VkDeviceSize bufferSize);

        void CopyImageBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
                             VkBuffer srcBuffer, VkImage image, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image,
                                   VkImageLayout oldLayout, VkImageLayout newLayout);
    } // namespace aux

#pragma endregion

#pragma region IBO

    class IBO {
      public:
        explicit IBO(VkPhysicalDevice physical, VkDevice logical);
        ~IBO();

        IBO(const IBO&) = delete;
        IBO& operator=(const IBO&) = delete;
        // IBO(IBO&& other) noexcept;
        // IBO& operator=(IBO&& other) noexcept;

        [[nodiscard]] size_t getCount() const { return this->count; }
        [[nodiscard]] VkBuffer getBuffer() const { return this->ibo->getBuffer(); }

        void destroy();
        void create(VkQueue queue, VkCommandPool commandBuffer, std::vector<uint32_t>* indices);

      private:
        size_t count;
        VkPhysicalDevice physical;
        VkDevice logical;
        std::shared_ptr<ce::BufferObject> ibo;
    };

#pragma endregion

#pragma region UBO

    template <typename T, template <typename, typename> class Container = std::vector>
    class UBO {
      public:
        explicit UBO(VkDevice logical) : logical(logical) {
            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(this->logical);
            this->descriptorSets = std::make_shared<DescriptorSet>(this->logical);
        }
        explicit UBO(VkPhysicalDevice physical, VkDevice logical, const size_t maxUBO, const size_t sizeDataUBO)
            : logical(logical) {

            // ViewProjection Buffer size
            const VkDeviceSize vpBufferSize = sizeDataUBO; // tamanho do struct com os dados

            // One uniform buffer for each image (and by extention, command buffer)
            this->ubo.resize(maxUBO); // total a ser criado

            // Create Unifor buffers
            for (size_t i = 0; i < maxUBO; i++) {
                this->ubo[i] = std::make_shared<T>(physical, logical);
                this->ubo[i]->create(vpBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            }

            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(this->logical);
            this->descriptorSets = std::make_shared<DescriptorSet>(this->logical);
        }

        virtual ~UBO() {

            // for (size_t i = 0; i < sizeDataUBO; i++) {
            //     this->ubo[i].reset();
            // }

            this->descriptorSets.reset(); // FIXME: acima da erro
            this->descriptorSetLayout.reset();
        }

        void addDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& vpLayoutBinding) {
            this->descriptorSetLayout->addBinding(vpLayoutBinding);
        }

        void createDescriptorSetLayout() { this->descriptorSetLayout->create(); }

        void addWriteDescriptorSet(const VkWriteDescriptorSet& vpSetWrite) { this->setWrites.push_back(vpSetWrite); }
        void clearWriteDescriptorSet() { this->setWrites.clear(); }

        std::pair<size_t, size_t> allocateDescriptorSets(size_t tot, const VkDescriptorPool& descriptorPool) {
            std::vector<VkDescriptorSetLayout> setLayouts(tot, this->descriptorSetLayout->get());
            return this->descriptorSets->allocate(descriptorPool, setLayouts);
        }

        void updateDescriptorSets() {
            // Update the descripto sets with new buffer/binding info
            vkUpdateDescriptorSets(logical, static_cast<uint32_t>(this->setWrites.size()), this->setWrites.data(), 0,
                                   nullptr);
        }

        [[nodiscard]] size_t size() const noexcept { return ubo.size(); }
        [[nodiscard]] std::vector<std::shared_ptr<T>>& getUBO() { return ubo; }
        [[nodiscard]] VkDescriptorSetLayout& getDescriptorSetLayout() const { return descriptorSetLayout->get(); }
        [[nodiscard]] std::vector<VkDescriptorSet>& getDescriptorSets() const { return descriptorSets->get(); }

      private:
        VkDevice logical;
        std::shared_ptr<DescriptorSet> descriptorSets;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        Container<std::shared_ptr<T>, std::allocator<std::shared_ptr<T>>> ubo;

        std::vector<VkWriteDescriptorSet> setWrites;
    };

#pragma endregion

#pragma region BufferDynamic

    struct UboModel {
        glm::mat4 model;
    };

    // class BufferDynamic { // TODO: validar se precisa existir
    //   public:
    //     explicit BufferDynamic(VkDevice logical) : logical(logical) {}
    //     ~BufferDynamic() = default;

    //   private:
    //     void allocateDynamicBufferTransferSpace() {

    //         // Caculate alignment of model data
    //         this->modelUniformAlignment =
    //             (sizeof(UboModel) + this->minUniformBufferOffset - 1) & ~(this->minUniformBufferOffset - 1);

    //         // Create space in memory to hold dynamic byffer that is alignment and holds MAX_OBJECTS
    //         this->modelTransferSpace =
    //             (UboModel*)aligned_alloc(this->modelUniformAlignment, this->modelUniformAlignment * MAX_OBJECTS);
    //     }

    //     void updateUniformBuffers(uint32_t imageIndex, const std::vector<Mesh>& meshList) {

    //         // Copy Model data
    //         void* data = nullptr;
    //         // for (size_t i = 0; i < meshList.size(); i++) {

    //         //     std::byte* ptr_base = reinterpret_cast<std::byte*>(this->modelTransferSpace);
    //         //     std::byte* ptr_atual = ptr_base + (i * this->modelUniformAlignment);
    //         //     UboModel* thisModel = std::launder(reinterpret_cast<UboModel*>(ptr_atual));

    //         //     *thisModel = meshList[i].getModel(); // FIXME: modelo antigo funcionava, reavaliar
    //         // }

    //         // Map the list of model data // FIXME: usar class BufferObject abaixo!!
    //         vkMapMemory(logical, this->modelDUniformBufferMemory[imageIndex], 0,
    //                     this->modelUniformAlignment * meshList.size(), 0, &data);
    //         std::memcpy(data, this->modelTransferSpace, this->modelUniformAlignment * meshList.size());
    //         vkUnmapMemory(logical, this->modelDUniformBufferMemory[imageIndex]);
    //     }

    //     VkDevice logical;

    //     VkDeviceSize minUniformBufferOffset;
    //     size_t modelUniformAlignment;
    //     UboModel* modelTransferSpace;

    //     std::vector<VkBuffer> modelDUniformBuffer;
    //     std::vector<VkDeviceMemory> modelDUniformBufferMemory;
    // };
#pragma endregion
} // namespace ce

#endif
