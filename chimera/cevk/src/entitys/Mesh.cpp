#include "entitys/Mesh.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace ce {

    Mesh::Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
               std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, int newTexId) {

        this->physicalDevice = newPhysicalDevice;
        this->device = newDevice;

        this->vbo = std::make_shared<VBO>(newPhysicalDevice, newDevice);
        this->vbo->create(transferQueue, transferCommandPool, vertices, sizeof(Vertex));

        this->ibo = std::make_shared<IBO>(newPhysicalDevice, newDevice);
        this->ibo->create(transferQueue, transferCommandPool, indices);

        this->model.model = glm::mat4(1.0F);
        this->texId = newTexId;
    }

    int Mesh::getVertexCount() const { return this->vbo->getCount(); }

    VkBuffer Mesh::getVertexBuffer() { return this->vbo->getBuffer(); }

    int Mesh::getIndexCount() const { return this->ibo->getCount(); }

    VkBuffer Mesh::getIndexBuffer() { return this->ibo->getBuffer(); }

    int Mesh::getTexId() const { return this->texId; }

    void Mesh::destroyBuffers() {
        this->vbo.reset();
        this->ibo.reset();
    }

    void Mesh::setModel(glm::mat4 newModel) { this->model.model = newModel; }

    Model Mesh::getModel() { return this->model; }
} // namespace ce
