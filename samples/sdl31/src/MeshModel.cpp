#include "MeshModel.hpp"
#include <assimp/material.h>
#include <assimp/types.h>

namespace ce {

    MeshModel::MeshModel(const std::vector<Mesh>& new_mesh_list) {
        meshList = new_mesh_list;
        model = glm::mat4(1.0F);
    }

    Mesh* MeshModel::getMesh(const size_t& index) {

        if (index > meshList.size()) {
            throw std::runtime_error("Attempted to access invalid Mesh index!");
        }

        return &this->meshList[index];
    }

    void MeshModel::destroyMeshModel() {
        for (auto& mesh : this->meshList) {
            mesh.destroy_buffers();
        }
    }
    std::vector<std::string> MeshModel::LoadMaterials(const aiScene* scene) {

        // Create 1:1 sized list of textures
        std::vector<std::string> texture_list(scene->mNumMaterials);

        // go through each material and copy its texture file name (if it exists)
        for (size_t i = 0; i < scene->mNumMaterials; i++) {

            // Get material
            aiMaterial* material = scene->mMaterials[i];

            // Inicialise the texture to empty string (will be replaced if texture exists)
            texture_list[i] = "";

            // Check for a Diffuse Texture (standard detail texture)
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

                // Get tha Path of the texture file
                aiString path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

                    // Cut off any directory information already present
                    // int idx = std::string(path.data).rfind('\\');
                    std::string filename = std::string(path.data); //.substr(idx);
                    // filename = "./textures/" + filename.erase(0, 1);
                    // filename.erase(0, 1);

                    texture_list[i] = filename;
                }
            }
        }

        return texture_list;
    }

    std::vector<Mesh> MeshModel::LoadNode(VkPhysicalDevice physical, VkDevice logical, VkQueue queue,
                                          VkCommandPool command_pool, aiNode* node, const aiScene* scene,
                                          std::vector<int>& mat_to_text) {
        //
        std::vector<Mesh> mesh_list;

        // Go through each mesh at this node and create it, then add it to our meshList
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            mesh_list.push_back(
                LoadMesh(physical, logical, queue, command_pool, scene->mMeshes[node->mMeshes[i]], scene, mat_to_text));
        }

        // Go through each attached to this node and load it, then append their meshes to this node's mesh list
        for (size_t i = 0; i < node->mNumChildren; i++) {
            //
            std::vector<Mesh> new_list =
                LoadNode(physical, logical, queue, command_pool, node->mChildren[i], scene, mat_to_text);
            mesh_list.insert(mesh_list.end(), new_list.begin(), new_list.end());
        }

        return mesh_list;
    }

    Mesh MeshModel::LoadMesh(VkPhysicalDevice physical, VkDevice logical, VkQueue queue, VkCommandPool command_pool,
                             aiMesh* mesh, const aiScene* scene, std::vector<int> mat_to_text) {
        //
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Resize vertex list to hold all vertices for mesh
        vertices.resize(mesh->mNumVertices);

        // Go through each vertex and copy it across to our vertice
        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            // Set Position
            vertices[i].pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            // Set Tex coord (if they exist)
            if (mesh->mTextureCoords[0] != nullptr) {
                vertices[i].tex = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else {
                vertices[i].tex = glm::vec2(0.0F, 0.0F);
            }

            // set colour (just use white for now)
            vertices[i].col = {1.0F, 1.0F, 1.0F};
        }

        // Iterator over indices through faces and copy across
        for (size_t i = 0; i < mesh->mNumFaces; i++) {

            // Get a face
            aiFace face = mesh->mFaces[i];

            // Go through face's indices and add to list
            for (size_t j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Create new Mesh with details and return it
        return {physical, logical, queue, command_pool, &vertices, &indices, mat_to_text[mesh->mMaterialIndex]};
    }

} // namespace ce
