#include "MeshModel.hpp"
#include <assimp/material.h>
#include <assimp/types.h>

namespace ce {

    MeshModel::MeshModel(const std::vector<Mesh>& newMeshList) {
        meshList = newMeshList;
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
            mesh.destroyBuffers();
        }
    }
    std::vector<std::string> MeshModel::LoadMaterials(const aiScene* scene) {

        // Create 1:1 sized list of textures
        std::vector<std::string> textureList(scene->mNumMaterials);

        // go through each material and copy its texture file name (if it exists)
        for (size_t i = 0; i < scene->mNumMaterials; i++) {

            // Get material
            aiMaterial* material = scene->mMaterials[i];

            // Inicialise the texture to empty string (will be replaced if texture exists)
            textureList[i] = "";

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

                    textureList[i] = filename;
                }
            }
        }

        return textureList;
    }

    std::vector<Mesh> MeshModel::LoadNode(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue,
                                          VkCommandPool transferCommandPool, aiNode* node, const aiScene* scene,
                                          std::vector<int>& matToText) {
        //
        std::vector<Mesh> meshList;

        // Go through each mesh at this node and create it, then add it to our meshList
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            meshList.push_back(LoadMesh(newPhysicalDevice, newDevice, transferQueue, transferCommandPool,
                                        scene->mMeshes[node->mMeshes[i]], scene, matToText));
        }

        // Go through each attached to this node and load it, then append their meshes to this node's mesh list
        for (size_t i = 0; i < node->mNumChildren; i++) {
            //
            std::vector<Mesh> newList = LoadNode(newPhysicalDevice, newDevice, transferQueue, transferCommandPool,
                                                 node->mChildren[i], scene, matToText);
            meshList.insert(meshList.end(), newList.begin(), newList.end());
        }

        return meshList;
    }

    Mesh MeshModel::LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue,
                             VkCommandPool transferCommandPool, aiMesh* mesh, const aiScene* scene,
                             std::vector<int> matToText) {
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
        return {newPhysicalDevice,
                newDevice,
                transferQueue,
                transferCommandPool,
                &vertices,
                &indices,
                matToText[mesh->mMaterialIndex]};
    }

} // namespace ce
