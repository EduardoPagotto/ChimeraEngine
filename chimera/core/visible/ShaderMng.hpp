#pragma once
#include "Shader.hpp"

namespace Chimera {

class ShaderMng {
  private:
    inline static std::unordered_map<std::string, std::shared_ptr<Shader>> map_shaders;

  public:
    static std::shared_ptr<Shader> load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles) noexcept;
    static void remove(std::shared_ptr<Shader> shader) noexcept;
    static void clear() noexcept;
};
} // namespace Chimera