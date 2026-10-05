#pragma once

#include "cevk/Mesh.hpp"
#include <assimp/scene.h>

namespace ce {

    struct Model {
        glm::mat4 model;
    };

    class MeshModel {
      public:
        MeshModel() = default;
        virtual ~MeshModel() = default;
        explicit MeshModel(const std::vector<Mesh>& new_mesh_list);

        [[nodiscard]] Mesh* get_mesh(const size_t& index);
        [[nodiscard]] size_t get_mesh_count() const { return mesh_list_.size(); }
        [[nodiscard]] glm::mat4 get_model() const { return this->model_; }
        [[nodiscard]] const glm::mat4& get_model2() const { return this->model_; }
        void set_model(glm::mat4 new_model) { this->model_ = new_model; }

        void destroy_mesh_model();

        static std::vector<std::string> load_materials(const aiScene* scene);

        static std::vector<Mesh> load_node(VkPhysicalDevice physical, VkDevice logical, VkQueue queue,
                                           VkCommandPool command_pool, aiNode* node, const aiScene* scene,
                                           std::vector<int>& mat_to_text);

        static Mesh load_mesh(VkPhysicalDevice physical, VkDevice logical, VkQueue queue, VkCommandPool command_pool,
                              aiMesh* mesh, const aiScene* scene, std::vector<int> mat_to_text);

      private:
        std::vector<Mesh> mesh_list_;
        glm::mat4 model_;
    };
} // namespace ce
