#include "Loader.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>
#include <glm/glm.hpp>

namespace ce {
    Loader::Loader(const std::filesystem::path& filePath) {

        // 1. Create the data buffer using the modern static constructor
        auto expectedBuffer = fastgltf::GltfDataBuffer::FromPath(filePath);

        // 2. Always validate that the file read succeeded
        if (expectedBuffer.error() != fastgltf::Error::None) {
            throw std::runtime_error("Failed to read file buffer into memory.");
        }

        fastgltf::Parser parser;
        constexpr auto options = fastgltf::Options::LoadExternalBuffers | fastgltf::Options::DecomposeNodeMatrices;

        // 3. Parse the asset using the buffer (.get() unpacks the expected value)
        auto expectedAsset = parser.loadGltf(expectedBuffer.get(), filePath.parent_path(), options);
        if (expectedAsset.error() != fastgltf::Error::None) {
            throw std::runtime_error("Failed to parse glTF structure");
        }

        // fastgltf::Asset asset = std::move(expectedAsset.get());
        this->asset = std::move(expectedAsset.get());

        this->getScene();
        this->getMaterials();
        this->getMeshs();
    }

    Loader::~Loader() {}

    void Loader::getScene() {
        //
        // Se houver cenas no glTF, pega a cena padrão (ou a primeira)
        if (!asset.scenes.empty()) {
            const auto& scene = asset.scenes.front(); // ou asset.scenes[asset.defaultScene]

            for (size_t nodeIndex : scene.nodeIndices) {
                // Inicia a recursão a partir dos nós-raiz da cena
                Loader::traverseNode(nodeIndex, glm::mat4(1.0F), sceneHierarchy);
            }
        }
    }

    // Função para ler a matriz de transformação de um Node glTF para um glm::mat4
    glm::mat4 Loader::getNodeTransform(const fastgltf::Node& node) {

        glm::mat4 nodeMatrix = glm::mat4(1.0F);

        if (const auto* matrix = std::get_if<fastgltf::math::fmat4x4>(&node.transform)) {
            // A matriz de transformação foi especificada diretamente no glTF
            std::memcpy(&nodeMatrix[0][0], matrix->data(), sizeof(float) * 16);
        } else if (const auto* transform = std::get_if<fastgltf::TRS>(&node.transform)) {
            // O nó usa os componentes translation, rotation e scale individualmente
            glm::vec3 translation(transform->translation[0], transform->translation[1], transform->translation[2]);
            glm::quat rotation(transform->rotation[3], transform->rotation[0], transform->rotation[1], transform->rotation[2]);
            glm::vec3 scale(transform->scale[0], transform->scale[1], transform->scale[2]);

            // Constrói a matriz final
            nodeMatrix = glm::translate(glm::mat4(1.0F), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0F), scale);
        }

        return nodeMatrix;
    }

    // Função recursiva que monta o grafo de cena
    void Loader::traverseNode(size_t nodeIndex, const glm::mat4& parentMatrix, std::vector<SceneNode>& outNodes) {
        const auto& node = asset.nodes[nodeIndex];

        SceneNode sceneNode;
        sceneNode.name = node.name;
        sceneNode.localTransform = getNodeTransform(node);
        // Matriz de Transformação Global = Matriz Global do Pai * Matriz Local do Filho
        sceneNode.globalTransform = parentMatrix * sceneNode.localTransform;

        // Vincula componentes se existirem
        if (node.meshIndex.has_value()) {
            sceneNode.meshIndex = node.meshIndex.value();
        }

        if (node.cameraIndex.has_value()) {
            sceneNode.cameraIndex = node.cameraIndex.value();
        }

        // Verificar se existe extensão de Luz (ex: KHR_lights_punctual)
        if (node.lightIndex.has_value()) {
            sceneNode.lightIndex = node.lightIndex.value();
        }

        // Processar filhos de forma recursiva
        for (size_t childIndex : node.children) {
            traverseNode(childIndex, sceneNode.globalTransform, sceneNode.children);
        }

        outNodes.push_back(sceneNode);
    }

    void Loader::getMeshs() {

        for (const auto& mesh : asset.meshes) {

            MeshData meshData;

            for (const auto& primitive : mesh.primitives) {

                // --- INDEX BUFFER ---
                if (primitive.indicesAccessor.has_value()) {
                    auto& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
                    meshData.indices.resize(indexAccessor.count);

                    fastgltf::iterateAccessorWithIndex<uint32_t>(asset, indexAccessor,
                                                                 [&](uint32_t idx, size_t size) { meshData.indices[size] = idx; });
                }

                // --- VERTEX BUFFER ---
                const auto* posAttr = primitive.findAttribute("POSITION");
                const auto* uvAttr = primitive.findAttribute("TEXCOORD_0");
                const auto* normalAttr = primitive.findAttribute("NORMAL");

                if (posAttr != primitive.attributes.end()) {
                    auto& posAccessor = asset.accessors[posAttr->accessorIndex];
                    meshData.vertices.resize(posAccessor.count);

                    // Carrega as posições
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, posAccessor, [&](glm::vec3 vertexPosition, size_t index) {
                        meshData.vertices[index].position = vertexPosition;
                    });
                }

                if (normalAttr != primitive.attributes.end()) {
                    auto& normalAccessor = asset.accessors[normalAttr->accessorIndex];

                    // Carrega normal
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset, normalAccessor, [&](glm::vec3 nor, size_t index) { meshData.vertices[index].normal = nor; });
                }

                if (uvAttr != primitive.attributes.end()) {
                    auto& uvAccessor = asset.accessors[uvAttr->accessorIndex];

                    // Carrega as UVs
                    fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, uvAccessor,
                                                                  [&](glm::vec2 uvd, size_t index) { meshData.vertices[index].uv = uvd; });
                }

                // --- TEXTURAS ---
                if (primitive.materialIndex.has_value()) {
                    // const auto& material = asset.materials[primitive.materialIndex.value()];
                    meshData.materialID = primitive.materialIndex.value();
                }
            }

            this->vMeshs.push_back(meshData);
        }
    }

    void Loader::getMaterials() {

        for (const auto& material : asset.materials) {

            MaterialData materialData;
            materialData.name = material.name;

            // Acessa o atalho PBR da nova API
            const auto& pbr = material.pbrData;

            materialData.metallic = pbr.metallicFactor;
            materialData.roughness = pbr.roughnessFactor;

            auto color = pbr.baseColorFactor;
            materialData.baseColorFactor = glm::vec4(color[0], color[1], color[2], color[3]);

            if (pbr.metallicRoughnessTexture.has_value()) {
                materialData.metallicRoughnessTexture.textureIndex = pbr.metallicRoughnessTexture->textureIndex;
                const auto& textureInfo = pbr.metallicRoughnessTexture.value();
                auto texture = asset.textures[textureInfo.textureIndex];

                if (texture.imageIndex.has_value()) {
                    const auto& image = asset.images[texture.imageIndex.value()];

                    // Caminho da imagem em disco ou arquivo embutido
                    if (std::holds_alternative<fastgltf::sources::URI>(image.data)) {
                        materialData.metallicRoughnessTexture.source = std::get<fastgltf::sources::URI>(image.data).uri.c_str();
                    }
                }
            }

            if (pbr.baseColorTexture.has_value()) {
                materialData.baseColorTexture.textureIndex = pbr.baseColorTexture->textureIndex;
                const auto& textureInfo = pbr.baseColorTexture.value();
                auto& texture = asset.textures[textureInfo.textureIndex];

                if (texture.imageIndex.has_value()) {
                    const auto& image = asset.images[texture.imageIndex.value()];

                    // Caminho da imagem em disco ou arquivo embutido
                    if (std::holds_alternative<fastgltf::sources::URI>(image.data)) {
                        materialData.baseColorTexture.source = std::get<fastgltf::sources::URI>(image.data).uri.c_str();
                    }
                }
            }

            this->vMaterial.push_back(materialData);
        }
    }
} // namespace ce
