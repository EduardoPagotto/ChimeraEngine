#pragma once
#include "Shader.hpp"
#include "chimera/base/ServiceLocator.hpp"

namespace ce {

    class ShaderMng : public IService {
      private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> map_shaders;

      public:
        explicit ShaderMng() noexcept = default;
        virtual ~ShaderMng() noexcept { this->clear(); }

        ShaderMng(const ShaderMng& o) = delete;
        ShaderMng& operator=(const ShaderMng& o) = delete;

        std::type_index getTypeIndex() const override { return std::type_index(typeid(ShaderMng)); }

        void clear() noexcept {
            for (auto it = map_shaders.begin(); it != map_shaders.end(); it++)
                it->second = nullptr;

            map_shaders.clear();
        }

        void remove(std::shared_ptr<Shader> shader) noexcept {
            for (auto it = map_shaders.begin(); it != map_shaders.end(); it++) {
                if (it->second == shader) {
                    it->second = nullptr;
                    map_shaders.erase(it);
                }
            }
        }

        std::shared_ptr<Shader> load(const std::string& name,
                                     const std::unordered_map<uint32_t, std::string>& mFiles) noexcept;
    };
} // namespace ce
