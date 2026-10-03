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

    struct Vertex3D {
        glm::vec3 pos{0.0};
        glm::vec3 nor{0.0};
        glm::vec2 tex{0.0};
    };

    struct MeshPart {
        // AABB aabb;
        std::vector<uint32_t> indices;
        std::optional<size_t> materialIndex;
    };

    struct CompleteMesh {
        std::string name;
        std::vector<Vertex3D> vertices;
        std::vector<MeshPart> parts; // Each part represents a glTF primitive
    };

    class Loader {
      public:
        explicit Loader(const std::filesystem::path& file_path, entt::registry* registry) : registry_(registry) {

            // 1. Create the data buffer using the modern static constructor
            auto expected_buffer = fastgltf::GltfDataBuffer::FromPath(file_path);

            // 2. Always validate that the file read succeeded
            if (expected_buffer.error() != fastgltf::Error::None) {
                throw std::runtime_error("Failed to read file buffer into memory.");
            }

            fastgltf::Parser parser(fastgltf::Extensions::KHR_lights_punctual);

            constexpr auto options = fastgltf::Options::LoadExternalBuffers | fastgltf::Options::DecomposeNodeMatrices |
                                     fastgltf::Options::DontRequireValidAssetMember;

            // constexpr auto extensions = fastgltf::Extensions::KHR_lights_punctual;

            // 3. Parse the asset using the buffer (.get() unpacks the expected value)
            auto expected_asset = parser.loadGltf(expected_buffer.get(), file_path.parent_path(), options);
            if (expected_asset.error() != fastgltf::Error::None) {
                throw std::runtime_error("Failed to parse glTF structure");
            }

            // fastgltf::Asset asset = std::move(expectedAsset.get());
            asset_ = std::move(expected_asset.get());
        }

        virtual ~Loader() = default;

        static std::pair<entt::id_type, std::string_view> get_identify(const fastgltf::Image& image) {

            if (const auto* val = std::get_if<fastgltf::sources::URI>(&image.data)) {
                std::string name = (!image.name.empty()) ? std::string(image.name) : std::string(val->uri.c_str());

                return {entt::hashed_string{name.c_str()}, val->uri.path()};
            }

            throw std::runtime_error("Falha no parse de textura");
        }

        void get_images(const std::filesystem::path& img_path, std::shared_ptr<VulkanContext> ctx) {
            auto& asset_manager = registry_->ctx().get<AssetManager>();

            size_t i = 0;
            for (const auto& image : asset_.images) {

                auto [id_textura, uri] = get_identify(image);

                auto fim = img_path / uri;

                SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Image (%lu) id: %u -> %s", i++, id_textura, fim.c_str());

                asset_manager.texture.load(id_textura, ctx, std::string(fim).c_str());

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

            std::vector<CompleteMesh> out_meshes;

            // 2. Iterate through all meshes within the asset
            for (const auto& mesh : asset_.meshes) {

                CompleteMesh complete_mesh;
                complete_mesh.name = mesh.name;

                uint32_t count_primitive = 0;
                // 3. Process every primitive (sub-mesh) inside this mesh
                for (const auto& primitive : mesh.primitives) {

                    // We only care about rendering triangles
                    if (primitive.type != fastgltf::PrimitiveType::Triangles) {
                        continue;
                    }

                    if (count_primitive == 0) { // VBO

                        // Verificando compartilhamento de Índices (Indices)
                        if (primitive.indicesAccessor.has_value()) {
                            size_t accessor_index = primitive.indicesAccessor.value();
                            std::cout << "Acessor de Índices: ID " << accessor_index << "\n";
                        }

                        for (const auto& [attributeName, accessorIndex] : primitive.attributes) {
                            // attributeName geralmente é uma string ou um tipo mapeável estruturado
                            std::cout << " - Nome: " << attributeName << " (Index do Accessor: " << accessorIndex
                                      << ")\n";
                        }

                        const auto* pos_attribute = primitive.findAttribute("POSITION");
                        const auto* norm_attribute = primitive.findAttribute("NORMAL");
                        const auto* uv_attribute = primitive.findAttribute("TEXCOORD_0");
                        // const auto* colorAttribute = primitive.findAttribute("COLOR_0");

                        // --- PROCESS VERTICES ---
                        // Find the core POSITION attribute accessor to determine the sizing requirement
                        if (pos_attribute == primitive.attributes.end()) {
                            continue; // Invalid primitive
                        }

                        const auto& pos_accessor = asset_.accessors[pos_attribute->accessorIndex];
                        size_t vertex_count = pos_accessor.count;
                        complete_mesh.vertices.resize(vertex_count);

                        // Fetch and map the POSITION attribute into GLM vec3
                        fastgltf::iterateAccessorWithIndex<glm::vec3>(
                            asset_, pos_accessor,
                            [&](glm::vec3 pos, size_t idx) { complete_mesh.vertices[idx].pos = pos; });

                        // Fetch and map the NORMAL attribute if present
                        if (norm_attribute != primitive.attributes.end()) {
                            const auto& norm_accessor = asset_.accessors[norm_attribute->accessorIndex];
                            fastgltf::iterateAccessorWithIndex<glm::vec3>(
                                asset_, norm_accessor,
                                [&](glm::vec3 norm, size_t idx) { complete_mesh.vertices[idx].nor = norm; });
                        }

                        // Fetch and map the TEXCOORD_0 (Texture Coordinates) attribute if present
                        if (uv_attribute != primitive.attributes.end()) {
                            const auto& uv_accessor = asset_.accessors[uv_attribute->accessorIndex];
                            fastgltf::iterateAccessorWithIndex<glm::vec2>(
                                asset_, uv_accessor,
                                [&](glm::vec2 uv, size_t idx) { complete_mesh.vertices[idx].tex = uv; });
                        }

                        // Fetch and map the COLOR_0 attribute if present
                        // if (colorAttribute != primitive.attributes.end()) {
                        //     const auto& colorAccessor = asset.accessors[colorAttribute->accessorIndex];

                        //     // glTF colors can be written as either vec3 (RGB) or vec4 (RGBA)
                        //     fastgltf::iterateAccessorWithIndex<glm::vec4>(
                        //         asset, colorAccessor,
                        //         [&](glm::vec4 color, size_t idx) { completeMesh.vertices[idx].col = color; });
                        // }
                    }

                    count_primitive++;

                    MeshPart mesh_part;

                    if (primitive.materialIndex.has_value()) {
                        mesh_part.materialIndex = primitive.materialIndex.value();
                    }

                    // --- PROCESS INDICES ---
                    // If the primitive is indexed (or has them automatically generated by our option flag)
                    if (primitive.indicesAccessor.has_value()) {
                        const auto& index_accessor = asset_.accessors[primitive.indicesAccessor.value()];
                        mesh_part.indices.resize(index_accessor.count);

                        // iterateAccessor automatically handles converting uint8, uint16, or uint32 data types up
                        // to standard uint32_t
                        fastgltf::iterateAccessorWithIndex<uint32_t>(
                            asset_, index_accessor,
                            [&](uint32_t index, size_t idx) { mesh_part.indices[idx] = index; });
                    }

                    complete_mesh.parts.push_back(std::move(mesh_part));
                }

                out_meshes.push_back(std::move(complete_mesh));
            }

            return out_meshes;
        }

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
        fastgltf::Asset asset_;
        entt::registry* registry_;
    };
} // namespace ce
