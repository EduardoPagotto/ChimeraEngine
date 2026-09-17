#pragma once

#include "TextureLoader.hpp"
#include "chimera_core/gl/ShaderLoader.hpp"
#include <entt/entt.hpp>
#include <entt/resource/cache.hpp>

namespace ce {

    class AssetManager {
      public:
        AssetManager() = default;

        void clearAll() {
            this->clearTexture();
            this->clearShaders();
        }

        // Carrega a textura a partir de uma chave string ID e o caminho do arquivo
        entt::resource<Texture> loadTexture(std::string_view stringId, const std::string& filepath, TexParam& tp) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();

            // Retorna a textura existente se já carregada, ou faz o trigger do Loader se não existir
            auto [it, inserted] = m_textureCache.load(hashedId, filepath, tp);
            return it->second;
        }

        // Cria uma textura usando uma surface
        entt::resource<Texture> addTextureFromSurface(std::string_view stringId, SDL_Surface* surface, TexParam& tp) {
            if (surface == nullptr) {
                throw std::runtime_error("Superfície SDL fornecida é nula.");
            }

            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();

            // Se já existir no cache com este ID, apenas retorna a existente
            if (m_textureCache.contains(hashedId)) {
                return m_textureCache[hashedId];
            }

            auto [it, inserted] = m_textureCache.force_load(hashedId, surface, tp);

            return it->second;
        }

        // Busca uma textura já mapeada pelo ID String
        entt::resource<Texture> getTexture(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            if (m_textureCache.contains(hashedId)) {
                return m_textureCache[hashedId];
            }
            throw std::runtime_error("[AssetManager] Textura não existe no cache: " + std::string(stringId));
        }

        // Remove do cache (se nenhum outro sistema guardar uma cópia do handle, ela sai da VRAM)
        void unloadTexture(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            m_textureCache.erase(hashedId);
        }

        void clearTexture() { m_textureCache.clear(); }

        // -- SHADERS

        // Carrega shader de lista de arquivos
        entt::resource<Shader> loadShader(std::string_view stringId,
                                          const std::unordered_map<uint32_t, std::string>& mFiles) {

            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();

            // Retorna a shader existente se já carregada, ou faz o trigger do Loader se não existir
            auto [it, inserted] = m_shaderCache.load(hashedId, mFiles);
            return it->second;
        }

        // Busca uma Shader já mapeada pelo ID String
        entt::resource<Shader> getShader(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            if (m_shaderCache.contains(hashedId)) {
                return m_shaderCache[hashedId];
            }
            throw std::runtime_error("[AssetManager] Shader nao exite no cache: " + std::string(stringId));
        }

        void unloadShader(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            m_shaderCache.erase(hashedId);
        }

        void clearShaders() { m_shaderCache.clear(); }

      private:
        using TextureCache = entt::resource_cache<Texture, TextureLoader>;
        using ShaderCache = entt::resource_cache<Shader, ShaderLoader>;
        TextureCache m_textureCache;
        ShaderCache m_shaderCache;
    };
} // namespace ce
