#pragma once

#include "AssetManager.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <entt/entt.hpp>
#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <variant>

namespace ce {

    // Define your application's Vertex structure
    // struct Vertex {
    //     glm::vec3 position{0.0f};
    //     glm::vec3 normal{0.0f};
    //     glm::vec2 texCoord{0.0f};
    //     glm::vec4 color{1.0f}; // Default to white if not provided
    // };

    // Define a structured representation for your parsed submesh/primitive
    // struct PrimitiveData {
    //     std::vector<Vertex> vertices;
    //     std::vector<uint32_t> indices;
    //     std::optional<std::size_t> materialIndex;
    // };

    // struct MeshData {
    //     std::string name;
    //     std::vector<PrimitiveData> primitives;
    // };

    struct MeshPart {
        std::vector<uint32_t> indices;
        size_t materialIndex;
    };

    struct CompleteMesh {
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<MeshPart> parts; // Each part represents a glTF primitive
    };

    class Loader {
      public:
        explicit Loader(const std::filesystem::path& filePath, entt::registry* registry) : registry(registry) {

            // 1. Create the data buffer using the modern static constructor
            auto expectedBuffer = fastgltf::GltfDataBuffer::FromPath(filePath);

            // 2. Always validate that the file read succeeded
            if (expectedBuffer.error() != fastgltf::Error::None) {
                throw std::runtime_error("Failed to read file buffer into memory.");
            }

            fastgltf::Parser parser(fastgltf::Extensions::KHR_lights_punctual);

            constexpr auto options = fastgltf::Options::LoadExternalBuffers | fastgltf::Options::DecomposeNodeMatrices |
                                     fastgltf::Options::DontRequireValidAssetMember;

            // constexpr auto extensions = fastgltf::Extensions::KHR_lights_punctual;

            // 3. Parse the asset using the buffer (.get() unpacks the expected value)
            auto expectedAsset = parser.loadGltf(expectedBuffer.get(), filePath.parent_path(), options);
            if (expectedAsset.error() != fastgltf::Error::None) {
                throw std::runtime_error("Failed to parse glTF structure");
            }

            // fastgltf::Asset asset = std::move(expectedAsset.get());
            this->asset = std::move(expectedAsset.get());
        }

        virtual ~Loader() = default;

        static std::pair<entt::id_type, std::string_view> GetIdentify(const fastgltf::Image& image) {

            if (const auto* val = std::get_if<fastgltf::sources::URI>(&image.data)) {
                std::string name = (!image.name.empty()) ? std::string(image.name) : std::string(val->uri.c_str());

                return {entt::hashed_string{name.c_str()}, val->uri.path()};
            }

            throw std::runtime_error("Falha no parse de textura");
        }

        void getImages(const std::filesystem::path& imgPath, std::shared_ptr<VulkanContext> ctx) {
            auto& assetManager = registry->ctx().get<AssetManager>();

            size_t i = 0;
            for (const auto& image : asset.images) {

                auto [id_textura, uri] = GetIdentify(image);

                auto fim = imgPath / uri;

                SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Image (%lu) id: %u -> %s", i++, id_textura, fim.c_str());

                assetManager.texture.load(id_textura, ctx, std::string(fim).c_str());

                // if (auto val = std::get_if<fastgltf::sources::URI>(&image.data)) {

                //     std::string name =
                //         (!image.name.empty()) ? std::string(image.name.c_str()) : std::string(val->uri.c_str());

                //     entt::id_type id_textura = entt::hashed_string{name.c_str()};

                //     auto fim = imgPath / val->uri.path();

                //     SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Image (%lu) name: %s id: %u -> %s", i++, name.c_str(),
                //                  id_textura, fim.c_str());

                //     assetManager.texture.load(id_textura, ctx, std::string(fim).c_str());
                // }
            }
        }

        std::vector<CompleteMesh> getMeshs(std::shared_ptr<VulkanContext> ctx) { // NOLINT
            // auto& assetManager = registry->ctx().get<AssetManager>();

            std::vector<CompleteMesh> outMeshes;

            // 2. Iterate through all meshes within the asset
            for (const auto& mesh : asset.meshes) {

                CompleteMesh completeMesh;
                completeMesh.name = mesh.name;

                uint32_t countPrimitive = 0;
                // 3. Process every primitive (sub-mesh) inside this mesh
                for (const auto& primitive : mesh.primitives) {

                    // We only care about rendering triangles
                    if (primitive.type != fastgltf::PrimitiveType::Triangles) {
                        continue;
                    }

                    if (countPrimitive == 0) { // VBO

                        // Verificando compartilhamento de Índices (Indices)
                        if (primitive.indicesAccessor.has_value()) {
                            size_t accessorIndex = primitive.indicesAccessor.value();
                            std::cout << "Acessor de Índices: ID " << accessorIndex << "\n";
                        }

                        for (const auto& [attributeName, accessorIndex] : primitive.attributes) {
                            // attributeName geralmente é uma string ou um tipo mapeável estruturado
                            std::cout << " - Nome: " << attributeName << " (Index do Accessor: " << accessorIndex
                                      << ")\n";
                        }

                        const auto* posAttribute = primitive.findAttribute("POSITION");
                        // const auto* normAttribute = primitive.findAttribute("NORMAL");
                        const auto* uvAttribute = primitive.findAttribute("TEXCOORD_0");
                        const auto* colorAttribute = primitive.findAttribute("COLOR_0");

                        // --- PROCESS VERTICES ---
                        // Find the core POSITION attribute accessor to determine the sizing requirement
                        if (posAttribute == primitive.attributes.end()) {
                            continue; // Invalid primitive
                        }

                        const auto& posAccessor = asset.accessors[posAttribute->accessorIndex];
                        size_t vertexCount = posAccessor.count;
                        completeMesh.vertices.resize(vertexCount);

                        // Fetch and map the POSITION attribute into GLM vec3
                        fastgltf::iterateAccessorWithIndex<glm::vec3>(
                            asset, posAccessor,
                            [&](glm::vec3 pos, size_t idx) { completeMesh.vertices[idx].pos = pos; });

                        // // Fetch and map the NORMAL attribute if present
                        // if (normAttribute != primitive.attributes.end()) {
                        //     const auto& normAccessor = asset.accessors[normAttribute->accessorIndex];
                        //     fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        //         asset, normAccessor,
                        //         [&](glm::vec3 norm, size_t idx) { completeMesh.vertices[idx].normal = norm; });
                        // }

                        // Fetch and map the TEXCOORD_0 (Texture Coordinates) attribute if present
                        if (uvAttribute != primitive.attributes.end()) {
                            const auto& uvAccessor = asset.accessors[uvAttribute->accessorIndex];
                            fastgltf::iterateAccessorWithIndex<glm::vec2>(
                                asset, uvAccessor,
                                [&](glm::vec2 uv, size_t idx) { completeMesh.vertices[idx].tex = uv; });
                        }

                        // Fetch and map the COLOR_0 attribute if present
                        if (colorAttribute != primitive.attributes.end()) {
                            const auto& colorAccessor = asset.accessors[colorAttribute->accessorIndex];

                            // glTF colors can be written as either vec3 (RGB) or vec4 (RGBA)
                            fastgltf::iterateAccessorWithIndex<glm::vec4>(
                                asset, colorAccessor,
                                [&](glm::vec4 color, size_t idx) { completeMesh.vertices[idx].col = color; });
                        }
                    }

                    countPrimitive++;

                    MeshPart meshPart;

                    if (primitive.materialIndex.has_value()) {
                        meshPart.materialIndex = primitive.materialIndex.value();
                    }

                    // --- PROCESS INDICES ---
                    // If the primitive is indexed (or has them automatically generated by our option flag)
                    if (primitive.indicesAccessor.has_value()) {
                        const auto& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
                        meshPart.indices.resize(indexAccessor.count);

                        // iterateAccessor automatically handles converting uint8, uint16, or uint32 data types up
                        // to standard uint32_t
                        fastgltf::iterateAccessorWithIndex<uint32_t>(
                            asset, indexAccessor, [&](uint32_t index, size_t idx) { meshPart.indices[idx] = index; });
                    }

                    completeMesh.parts.push_back(std::move(meshPart));
                }

                outMeshes.push_back(std::move(completeMesh));
            }

            return outMeshes;
        }

        // std::vector<MeshData> getMeshs(std::shared_ptr<VulkanContext> ctx) { // NOLINT
        //     // auto& assetManager = registry->ctx().get<AssetManager>();

        //     std::vector<MeshData> outMeshes;

        //     // 2. Iterate through all meshes within the asset
        //     for (const auto& mesh : asset.meshes) {
        //         MeshData currentMesh;
        //         currentMesh.name = mesh.name;

        //         // 3. Process every primitive (sub-mesh) inside this mesh
        //         for (const auto& primitive : mesh.primitives) {
        //             // We only care about rendering triangles
        //             if (primitive.type != fastgltf::PrimitiveType::Triangles) {
        //                 continue;
        //             }

        //             // Verificando compartilhamento de Índices (Indices)
        //             if (primitive.indicesAccessor.has_value()) {
        //                 size_t accessorIndex = primitive.indicesAccessor.value();
        //                 std::cout << "Acessor de Índices: ID " << accessorIndex << "\n";
        //             }

        //             for (const auto& [attributeName, accessorIndex] : primitive.attributes) {
        //                 // attributeName geralmente é uma string ou um tipo mapeável estruturado
        //                 std::cout << " - Nome: " << attributeName << " (Index do Accessor: " << accessorIndex <<
        //                 ")\n";
        //             }

        //             PrimitiveData currentPrim;

        //             if (primitive.materialIndex.has_value()) {
        //                 currentPrim.materialIndex = primitive.materialIndex.value();
        //             }

        //             const auto* posAttribute = primitive.findAttribute("POSITION");
        //             // const auto* normAttribute = primitive.findAttribute("NORMAL");
        //             const auto* uvAttribute = primitive.findAttribute("TEXCOORD_0");
        //             const auto* colorAttribute = primitive.findAttribute("COLOR_0");

        //             // --- PROCESS VERTICES ---
        //             // Find the core POSITION attribute accessor to determine the sizing requirement
        //             if (posAttribute == primitive.attributes.end()) {
        //                 continue; // Invalid primitive
        //             }

        //             const auto& posAccessor = asset.accessors[posAttribute->accessorIndex];
        //             size_t vertexCount = posAccessor.count;
        //             currentPrim.vertices.resize(vertexCount);

        //             // Fetch and map the POSITION attribute into GLM vec3
        //             fastgltf::iterateAccessorWithIndex<glm::vec3>(
        //                 asset, posAccessor, [&](glm::vec3 pos, size_t idx) { currentPrim.vertices[idx].pos = pos; });

        //             // Fetch and map the NORMAL attribute if present
        //             // if (normAttribute != primitive.attributes.end()) {
        //             //     const auto& normAccessor = asset.accessors[normAttribute->accessorIndex];
        //             //     fastgltf::iterateAccessorWithIndex<glm::vec3>(
        //             //         asset, normAccessor,
        //             //         [&](glm::vec3 norm, size_t idx) { currentPrim.vertices[idx].normal = norm; });
        //             // }

        //             // Fetch and map the TEXCOORD_0 (Texture Coordinates) attribute if present
        //             if (uvAttribute != primitive.attributes.end()) {
        //                 const auto& uvAccessor = asset.accessors[uvAttribute->accessorIndex];
        //                 fastgltf::iterateAccessorWithIndex<glm::vec2>(
        //                     asset, uvAccessor, [&](glm::vec2 uv, size_t idx) { currentPrim.vertices[idx].tex = uv;
        //                     });
        //             }

        //             // Fetch and map the COLOR_0 attribute if present
        //             if (colorAttribute != primitive.attributes.end()) {
        //                 const auto& colorAccessor = asset.accessors[colorAttribute->accessorIndex];

        //                 // glTF colors can be written as either vec3 (RGB) or vec4 (RGBA)
        //                 fastgltf::iterateAccessorWithIndex<glm::vec4>(
        //                     asset, colorAccessor,
        //                     [&](glm::vec4 color, size_t idx) { currentPrim.vertices[idx].col = color; });
        //             }

        //             // --- PROCESS INDICES ---
        //             // If the primitive is indexed (or has them automatically generated by our option flag)
        //             if (primitive.indicesAccessor.has_value()) {
        //                 const auto& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
        //                 currentPrim.indices.resize(indexAccessor.count);

        //                 // iterateAccessor automatically handles converting uint8, uint16, or uint32 data types up
        //                 // to standard uint32_t
        //                 fastgltf::iterateAccessorWithIndex<uint32_t>(
        //                     asset, indexAccessor,
        //                     [&](uint32_t index, size_t idx) { currentPrim.indices[idx] = index; });
        //             }

        //             currentMesh.primitives.push_back(std::move(currentPrim));
        //         }

        //         outMeshes.push_back(std::move(currentMesh));
        //     }

        //     return outMeshes;
        // }

        // void getMaterials() {

        //     for (const auto& material : asset.materials) {

        //         MaterialData materialData;
        //         materialData.name = material.name;

        //         // Acessa o atalho PBR da nova API
        //         const auto& pbr = material.pbrData;

        //         materialData.metallic = pbr.metallicFactor;
        //         materialData.roughness = pbr.roughnessFactor;

        //         auto color = pbr.baseColorFactor;
        //         materialData.baseColorFactor = glm::vec4(color[0], color[1], color[2], color[3]);

        //         materialData.emissiveFactor.x = material.emissiveFactor.x();
        //         materialData.emissiveFactor.y = material.emissiveFactor.y();
        //         materialData.emissiveFactor.z = material.emissiveFactor.z();

        //         if (pbr.metallicRoughnessTexture.has_value()) {
        //             materialData.metallicRoughnessTexture = pbr.metallicRoughnessTexture->textureIndex;
        //         }

        //         if (pbr.baseColorTexture.has_value()) {
        //             materialData.baseColorTexture = pbr.baseColorTexture->textureIndex;
        //         }

        //         this->vMaterial.push_back(materialData);
        //     }
        // }

      private:
        fastgltf::Asset asset;
        entt::registry* registry;
    };
} // namespace ce
