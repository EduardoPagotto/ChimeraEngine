#pragma once

#include <fastgltf/types.hpp>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct TextureDef {
    std::string source;
    uint32_t textureIndex;
    uint32_t texID;
};

struct MaterialData {
    std::string name;
    float metallic;
    float roughness;
    glm::vec4 baseColorFactor;
    TextureDef metallicRoughnessTexture;
    TextureDef baseColorTexture;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t materialID;
};

struct Model {
    glm::mat4 transformMatrix;
    std::vector<MeshData> meshes;
};

struct SceneNode {
    std::string name;
    glm::mat4 localTransform;
    glm::mat4 globalTransform;

    std::optional<size_t> meshIndex;
    std::optional<size_t> cameraIndex;
    std::optional<size_t> lightIndex;

    std::vector<SceneNode> children;
};

namespace ce {

    class Loader {
      public:
        explicit Loader(const std::filesystem::path& filePath);
        virtual ~Loader();

        void testMat();
        void testMesh();

      private:
        static glm::mat4 getNodeTransform(const fastgltf::Node& node);

        void traverseNode(size_t nodeIndex, const glm::mat4& parentMatrix, std::vector<SceneNode>& outNodes);

        void getScene();
        void getMaterials();
        void getMeshs();

        void getImages();

        fastgltf::Asset asset;
        std::vector<SceneNode> sceneHierarchy;
        std::vector<MeshData> vMeshs;
        std::vector<MaterialData> vMaterial;
        // std::vecto

        void textureDefDebug(TextureDef& t);
    };
} // namespace ce
