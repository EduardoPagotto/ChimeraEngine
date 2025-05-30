#pragma once
#include "ColladaDom.hpp"
#include "RFC3986.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ce {

class Collada {
  public:
    Collada(ColladaDom& dom, const std::string& url);
    virtual ~Collada() = default;
    const pugi::xml_node getLibraryUrl(const std::string& libraryName, const std::string& url);
    const pugi::xml_node getLibrary(const std::string& libraryName);
    static uint32_t getNewSerial() { return ++serial; }
    static void destroy();
    inline static std::vector<ColladaDom> vColladaDom;

  protected:
    ColladaDom colladaDom;
    inline static uint32_t serial;
    std::string fragment;

  private:
    const pugi::xml_node getLibraryKey(const std::string& libraryName, const std::string& key);
};

const glm::vec3 textToVec3(const std::string& text);
const glm::vec4 textToVec4(const std::string& text);
const glm::mat4 textToMat4(const std::string& text);
const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name);

template <typename T>
inline void setChildParam(const pugi::xml_node& node, const char* paramName, T& value) {
    // ... necessario pos c++ precisa de um escape generico se tipo nao definido
}

template <>
inline void setChildParam<bool>(const pugi::xml_node& node, const char* paramName, bool& value) {
    if (pugi::xml_node n = node.child(paramName); n != nullptr)
        value = n.text().as_bool();
}

template <>
inline void setChildParam<uint32_t>(const pugi::xml_node& node, const char* paramName, uint32_t& value) {
    if (pugi::xml_node n = node.child(paramName); n != nullptr)
        value = n.text().as_int();
}

template <>
inline void setChildParam<float>(const pugi::xml_node& node, const char* paramName, float& value) {
    if (pugi::xml_node n = node.child(paramName); n != nullptr)
        value = n.text().as_float();
}

template <>
inline void setChildParam<std::string>(const pugi::xml_node& node, const char* paramName, std::string& value) {
    if (pugi::xml_node n = node.child(paramName); n != nullptr)
        value = n.text().as_string();
}

template <>
inline void setChildParam<glm::vec3>(const pugi::xml_node& node, const char* paramName, glm::vec3& value) {
    if (pugi::xml_node n = node.child(paramName); n != nullptr)
        value = textToVec3(n.text().as_string());
}

} // namespace ce