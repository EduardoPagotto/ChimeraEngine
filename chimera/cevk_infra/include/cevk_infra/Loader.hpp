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
#include <stdexcept>
#include <string_view>
#include <variant>

namespace ce {

    class Loader {
      public:
        explicit Loader(const std::filesystem::path& filePath, entt::registry* registry) : registry(registry) {

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
        }

        virtual ~Loader() = default;

        std::pair<entt::id_type, std::string_view> getIdentify(const fastgltf::Image& image) {

            if (auto val = std::get_if<fastgltf::sources::URI>(&image.data)) {
                std::string name =
                    (!image.name.empty()) ? std::string(image.name.c_str()) : std::string(val->uri.c_str());

                return {entt::hashed_string{name.c_str()}, val->uri.path()};
            }

            throw std::runtime_error("Falha no parse de textura");
        }

        void getImages(const std::filesystem::path& imgPath, std::shared_ptr<VulkanContext> ctx) {
            auto& assetManager = registry->ctx().get<AssetManager>();

            size_t i = 0;
            for (const auto& image : asset.images) {

                auto [id_textura, uri] = getIdentify(image);

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
