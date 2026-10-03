#pragma once

#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace ce {

    class Frame {
      public:
        VkCommandPool commandPool{VK_NULL_HANDLE};
        VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
        VkSemaphore imageAvailableSemaphore{VK_NULL_HANDLE};
        VkSemaphore renderFinishedSemaphore{VK_NULL_HANDLE};
        VkFence inFlightFence{VK_NULL_HANDLE};

        explicit Frame() = default;
        explicit Frame(VkDevice device, uint32_t graphics_queue_family_index) {
            init(device, graphics_queue_family_index);
        }
        virtual ~Frame() { destroy(); }

        // Proibir cópia
        // Frame(const Frame&) = delete;
        // Frame& operator=(const Frame&) = delete;

        void init(VkDevice device, uint32_t graphics_queue_family_index) {

            device_ = device;

            // Definir as propriedades de criação do Command Pool
            const VkCommandPoolCreateInfo pool_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                // Permite que command buffers individuais sejam reiniciados via vkResetCommandBuffer
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = graphics_queue_family_index};

            // Criar Pool de Comandos
            if (vkCreateCommandPool(device_, &pool_info, nullptr, &commandPool) != VK_SUCCESS) {
                throw std::runtime_error("Falha ao criar Command Pool!");
            }

            // Alocar Command Buffer usando a Pool recém-criada
            // Definição das configurações de alocação do Command Buffer
            const VkCommandBufferAllocateInfo alloc_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = commandPool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // Buffer primário enviado direto para a Fila (Queue)
                .commandBufferCount = 1};

            if (vkAllocateCommandBuffers(device_, &alloc_info, &commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Falha ao alocar Command Buffer!");
            }

            // Configurações padrão dos Semáforos e Fences
            const VkSemaphoreCreateInfo semaphore_info{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
            const VkFenceCreateInfo fence_info = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                                  .flags = VK_FENCE_CREATE_SIGNALED_BIT};

            // Criar Semáforos (Sincronização entre operações da própria GPU)
            // Criar Fence (Sincronização Bloqueante entre CPU e GPU)
            if ((vkCreateSemaphore(device_, &semaphore_info, nullptr, &imageAvailableSemaphore) != VK_SUCCESS) ||
                (vkCreateSemaphore(device_, &semaphore_info, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) ||
                (vkCreateFence(device_, &fence_info, nullptr, &inFlightFence) != VK_SUCCESS)) {
                throw std::runtime_error("Fail to create Semaphores/Fence");
            }
        }

        void destroy() {
            if (device_ != VK_NULL_HANDLE) {
                if (imageAvailableSemaphore != VK_NULL_HANDLE) {
                    vkDestroySemaphore(device_, imageAvailableSemaphore, nullptr);
                }

                if (renderFinishedSemaphore != VK_NULL_HANDLE) {
                    vkDestroySemaphore(device_, renderFinishedSemaphore, nullptr);
                }

                if (inFlightFence != VK_NULL_HANDLE) {
                    vkDestroyFence(device_, inFlightFence, nullptr);
                }

                if (commandPool != VK_NULL_HANDLE) { // commandBuffer -> Destrói o buffer automaticamente
                    vkDestroyCommandPool(device_, commandPool, nullptr);
                }

                device_ = VK_NULL_HANDLE;
            }
        }

      private:
        VkDevice device_;
    }; // namespace ce

} // namespace ce
