#pragma once
#include "Shader.hpp"
#include "chimera/core/ServiceLocator.hpp"

namespace Chimera {

class ShaderMng : public IService {
  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> map_shaders;

  public:
    explicit ShaderMng() noexcept = default;
    virtual ~ShaderMng() noexcept { this->clear(); }

    ShaderMng(const ShaderMng& o) = delete;
    ShaderMng& operator=(const ShaderMng& o) = delete;

    std::shared_ptr<Shader> load(const std::string& name, const std::unordered_map<uint32_t, std::string>& mFiles) noexcept;
    void remove(std::shared_ptr<Shader> shader) noexcept;
    void clear() noexcept;
    std::type_index getTypeIndex() const { return std::type_index(typeid(ShaderMng)); }
};
} // namespace Chimera