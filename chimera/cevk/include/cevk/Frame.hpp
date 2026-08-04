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
        explicit Frame(VkDevice device, uint32_t graphicsQueueFamilyIndex) {
            this->init(device, graphicsQueueFamilyIndex);
        }
        virtual ~Frame() { destroy(); }

        // Proibir cópia
        // Frame(const Frame&) = delete;
        // Frame& operator=(const Frame&) = delete;

        void init(VkDevice device, uint32_t graphicsQueueFamilyIndex) {

            this->device = device;

            // Definir as propriedades de criação do Command Pool
            const VkCommandPoolCreateInfo poolInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                // Permite que command buffers individuais sejam reiniciados via vkResetCommandBuffer
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = graphicsQueueFamilyIndex};

            // Criar Pool de Comandos
            if (vkCreateCommandPool(this->device, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) {
                throw std::runtime_error("Falha ao criar Command Pool!");
            }

            // Alocar Command Buffer usando a Pool recém-criada
            // Definição das configurações de alocação do Command Buffer
            const VkCommandBufferAllocateInfo allocInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = this->commandPool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // Buffer primário enviado direto para a Fila (Queue)
                .commandBufferCount = 1};

            if (vkAllocateCommandBuffers(this->device, &allocInfo, &this->commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Falha ao alocar Command Buffer!");
            }

            // Configurações padrão dos Semáforos e Fences
            const VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
            const VkFenceCreateInfo fenceInfo = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                                 .flags = VK_FENCE_CREATE_SIGNALED_BIT};

            // Criar Semáforos (Sincronização entre operações da própria GPU)
            // Criar Fence (Sincronização Bloqueante entre CPU e GPU)
            if ((vkCreateSemaphore(this->device, &semaphoreInfo, nullptr, &this->imageAvailableSemaphore) !=
                 VK_SUCCESS) ||
                (vkCreateSemaphore(this->device, &semaphoreInfo, nullptr, &this->renderFinishedSemaphore) !=
                 VK_SUCCESS) ||
                (vkCreateFence(this->device, &fenceInfo, nullptr, &this->inFlightFence) != VK_SUCCESS)) {
                throw std::runtime_error("Fail to create Semaphores/Fence");
            }
        }

        void destroy() {
            if (device) {
                if (imageAvailableSemaphore)
                    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

                if (renderFinishedSemaphore)
                    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);

                if (inFlightFence)
                    vkDestroyFence(device, inFlightFence, nullptr);

                if (commandPool) // commandBuffer -> Destrói o buffer automaticamente
                    vkDestroyCommandPool(device, commandPool, nullptr);

                device = VK_NULL_HANDLE;
            }
        }

      private:
        VkDevice device;
    }; // namespace ce

} // namespace ce
