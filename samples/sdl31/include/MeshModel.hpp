#pragma once

#include "cevk/Mesh.hpp"
#include <assimp/scene.h>

namespace ce {

    class MeshModel {
      public:
        MeshModel() = default;
        virtual ~MeshModel() = default;
        explicit MeshModel(const std::vector<Mesh>& newMeshList);

        [[nodiscard]] Mesh* getMesh(const size_t& index);
        [[nodiscard]] size_t getMeshCount() const { return meshList.size(); }
        [[nodiscard]] glm::mat4 getModel() const { return this->model; }
        [[nodiscard]] const glm::mat4& getModel2() const { return this->model; }
        void setModel(glm::mat4 newModel) { this->model = newModel; }

        void destroyMeshModel();

        static std::vector<std::string> LoadMaterials(const aiScene* scene);

        static std::vector<Mesh> LoadNode(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue,
                                          VkCommandPool transferCommandPool, aiNode* node, const aiScene* scene,
                                          std::vector<int>& matToText);

        static Mesh LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue,
                             VkCommandPool transferCommandPool, aiMesh* mesh, const aiScene* scene,
                             std::vector<int> matToText);

      private:
        std::vector<Mesh> meshList;
        glm::mat4 model;
    };
} // namespace ce
