#ifndef __SHADER_VALUE__HPP
#define __SHADER_VALUE__HPP

#include <string>

#include "Shader.hpp"
#include "glm/glm.hpp"

namespace Chimera {

class ShaderValue {
  public:
    ShaderValue(const std::string& _name) : name(_name) {}
    ShaderValue(const ShaderValue& _cpy) { name = _cpy.name; }
    virtual void setUniform(Shader* _shader) = 0;
    std::string getName() const { return name; }

  protected:
    std::string name;
};

class ShaderValue4vf : public ShaderValue {
  public:
    ShaderValue4vf(const std::string& _name, const glm::vec4& _value) : ShaderValue(_name), value(_value) {}
    ShaderValue4vf(const ShaderValue4vf& _cpy) : ShaderValue(_cpy) { value = _cpy.value; }
    virtual inline void setUniform(Shader* _shader) override { _shader->setUniform4fv(name.c_str(), 1, (float*)&value[0]); }

  private:
    glm::vec4 value;
};

class ShaderValue1vf : public ShaderValue {
  public:
    ShaderValue1vf(const std::string& _name, const float& _value) : ShaderValue(_name), value(_value) {}
    ShaderValue1vf(const ShaderValue1vf& _cpy) : ShaderValue(_cpy) { value = _cpy.value; }
    virtual inline void setUniform(Shader* _shader) override { _shader->setUniform1fv(name.c_str(), 1, (float*)&value); }

  private:
    float value;
};

class ShaderValue1i : public ShaderValue {
  public:
    ShaderValue1i(const std::string& _name, const int& _value) : ShaderValue(_name), value(_value) {}
    ShaderValue1i(const ShaderValue1i& _cpy) : ShaderValue(_cpy) { value = _cpy.value; }
    virtual inline void setUniform(Shader* _shader) override { _shader->setUniform1i(name.c_str(), value); }

  private:
    int value;
};

// class ShaderValueTex : public ShaderValue {
//   public:
//     ShaderValueTex(const std::string& _name, Tex* _pTex, const unsigned& _indexTextureNumber)
//         : ShaderValue(_name), pTex(_pTex), indexTextureNumber(_indexTextureNumber) {}
//     virtual void setUniform(Shader* _shader) override { pTex->apply(indexTextureNumber, name, _shader); }

//   private:
//     unsigned indexTextureNumber;
//     Tex* pTex;
// };

} // namespace Chimera

#endif