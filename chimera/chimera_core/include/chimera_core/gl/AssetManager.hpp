#pragma once

#include "TextureLoader.hpp"
#include <entt/entt.hpp>

namespace ce {

    class AssetManager {
      public:
        AssetManager() = default;

        // Carrega a textura a partir de uma chave string ID e o caminho do arquivo
        entt::resource<Texture> loadTexture(std::string_view stringId, const std::string& filepath, TexParam& tp) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();

            // Retorna a textura existente se já carregada, ou faz o trigger do Loader se não existir
            auto [it, inserted] = m_textureCache.load(hashedId, filepath, tp);
            return it->second;
        }

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

            std::cout << "[AssetManager] Textura criada via SDL_Surface e registrada com ID: " << stringId << "\n";
            return it->second;
        }

        // Busca uma textura já mapeada pelo ID String
        entt::resource<Texture> getTexture(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            if (m_textureCache.contains(hashedId)) {
                return m_textureCache[hashedId];
            }
            throw std::runtime_error("Textura requisitada não encontrada no Cache: " + std::string(stringId));
        }

        // Remove do cache (se nenhum outro sistema guardar uma cópia do handle, ela sai da VRAM)
        void unloadTexture(std::string_view stringId) {
            auto hashedId = entt::hashed_string(std::string(stringId).c_str()).value();
            m_textureCache.erase(hashedId);
        }

        void clear() { m_textureCache.clear(); }

      private:
        using TextureCache = entt::resource_cache<Texture, TextureLoader>;
        TextureCache m_textureCache;
    };
} // namespace ce
