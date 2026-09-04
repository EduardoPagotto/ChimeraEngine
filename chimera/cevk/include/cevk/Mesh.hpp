#pragma once

#include "IBO.hpp"
#include "VBO.hpp"
#include "cevk.hpp"
#include <memory>

namespace ce {

    struct Model {
        glm::mat4 model;
    };

    class Mesh {
      public:
        Mesh() = default;
        Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue,
             VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices,
             int newTexId);

        virtual ~Mesh() = default;

        void setModel(glm::mat4 newModel);
        Model getModel();

        int getTexId() const;

        const Model& getModel2() const { return this->model; }

        size_t getVertexCount() const;
        VkBuffer getVertexBuffer();

        size_t getIndexCount() const;
        VkBuffer getIndexBuffer();

        void destroyBuffers();

      private:
        Model model;

        int texId;

        VkPhysicalDevice physicalDevice;
        VkDevice device;

        std::shared_ptr<VBO> vbo;
        std::shared_ptr<IBO> ibo;

        void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool,
                                std::vector<Vertex>* vertices);
        void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool,
                               std::vector<uint32_t>* indices);
    };
} // namespace ce
