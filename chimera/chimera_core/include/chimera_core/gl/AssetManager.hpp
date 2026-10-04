#pragma once

#include "TextureLoader.hpp"
#include "chimera_core/gl/Font.hpp"
#include "chimera_core/gl/FontLoader.hpp"
#include "chimera_core/gl/ShaderLoader.hpp"
#include <entt/entt.hpp>
#include <entt/resource/cache.hpp>
#include <ranges>

namespace ce {

    class AssetManager {
      public:
        AssetManager() = default;

        void clear_all() {
            this->clear_texture();
            this->clear_shaders();
            this->clear_fonts();
        }

        // Carrega a textura a partir de uma chave string ID e o caminho do arquivo
        entt::resource<Texture> load_texture(std::string_view string_id, const std::string& filepath, TexParam& tp) {
            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();

            // Retorna a textura existente se já carregada, ou faz o trigger do Loader se não existir
            auto [it, inserted] = texture_cache_.load(hashed_id, filepath, tp);
            return it->second;
        }

        // Cria uma textura usando uma surface
        entt::resource<Texture> add_texture_from_surface(std::string_view string_id, SDL_Surface* surface,
                                                         TexParam& tp) {
            if (surface == nullptr) {
                throw std::runtime_error("Superfície SDL fornecida é nula.");
            }

            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();

            // Se já existir no cache com este ID, apenas retorna a existente
            if (texture_cache_.contains(hashed_id)) {
                return texture_cache_[hashed_id];
            }

            auto [it, inserted] = texture_cache_.force_load(hashed_id, surface, tp);

            return it->second;
        }

        // Busca uma textura já mapeada pelo ID String
        entt::resource<Texture> get_texture(std::string_view string_id) {
            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();
            if (texture_cache_.contains(hashed_id)) {
                return texture_cache_[hashed_id];
            }
            throw std::runtime_error("[AssetManager] Textura não existe no cache: " + std::string(string_id));
        }

        // Remove do cache (se nenhum outro sistema guardar uma cópia do handle, ela sai da VRAM)
        void unload_texture(std::string_view string_id) {
            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();
            texture_cache_.erase(hashed_id);
        }

        entt::resource<Texture> get_texture_from_index(int indice) {
            auto [key, val] = *(texture_cache_ | std::ranges::views::drop(indice)).begin();

            return val;
        }

        void clear_texture() { texture_cache_.clear(); }

        // -- SHADERS

        // Carrega shader de lista de arquivos
        entt::resource<Shader> load_shader(std::string_view string_id,
                                           const std::unordered_map<uint32_t, std::string>& files) {

            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();

            // Retorna a shader existente se já carregada, ou faz o trigger do Loader se não existir
            auto [it, inserted] = shader_cache_.load(hashed_id, files);
            return it->second;
        }

        // Busca uma Shader já mapeada pelo ID String
        entt::resource<Shader> get_shader(std::string_view string_id) {
            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();
            if (shader_cache_.contains(hashed_id)) {
                return shader_cache_[hashed_id];
            }
            throw std::runtime_error("[AssetManager] Shader nao exite no cache: " + std::string(string_id));
        }

        void unload_shader(std::string_view string_id) {
            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();
            shader_cache_.erase(hashed_id);
        }

        void clear_shaders() { shader_cache_.clear(); }

        // -- Font
        entt::resource<Font> load_font(std::string_view string_id, const std::string& filepath, int size) {

            auto hashed_id = entt::hashed_string(std::string(string_id).c_str()).value();

            auto [it, inserted] = font_cache_.load(hashed_id, filepath, size);
            return it->second;
        }

        entt::resource<Font> get_font_from_index(int indice) {
            auto [key, val] = *(font_cache_ | std::ranges::views::drop(indice)).begin();

            return val;
        }

        void clear_fonts() { font_cache_.clear(); }

      private:
        using TextureCache = entt::resource_cache<Texture, TextureLoader>;
        using ShaderCache = entt::resource_cache<Shader, ShaderLoader>;
        using FontCache = entt::resource_cache<Font, FontLoader>;

        TextureCache texture_cache_;
        ShaderCache shader_cache_;
        FontCache font_cache_;
    };
} // namespace ce
