#pragma once
#include "ColladaDom.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace ce {

    class Collada {
      public:
        Collada(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url);
        virtual ~Collada() = default;
        const pugi::xml_node get_library_url(const std::string& library_name, const std::string& url);
        const pugi::xml_node get_library(const std::string& library_name);
        static uint32_t get_new_serial() { return ++serial; }
        static void destroy();
        inline static std::vector<ColladaDom> v_collada_dom;

      protected:
        std::shared_ptr<entt::registry> registry;
        ColladaDom colladaDom;
        inline static uint32_t serial;
        std::string fragment_;

      private:
        const pugi::xml_node get_library_key(const std::string& library_name, const std::string& key);
    };

    const glm::vec3 textToVec3(const std::string& text);
    const glm::vec4 textToVec4(const std::string& text);
    const glm::mat4 textToMat4(const std::string& text);
    const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name);

    template <typename T>
    inline void setChildParam(const pugi::xml_node& node, const char* param_name, T& value) {
        // ... necessario pos c++ precisa de um escape generico se tipo nao definido
    }

    template <>
    inline void setChildParam<bool>(const pugi::xml_node& node, const char* param_name, bool& value) {
        if (pugi::xml_node n = node.child(param_name); n != nullptr)
            value = n.text().as_bool();
    }

    template <>
    inline void setChildParam<uint32_t>(const pugi::xml_node& node, const char* param_name, uint32_t& value) {
        if (pugi::xml_node n = node.child(param_name); n != nullptr)
            value = n.text().as_int();
    }

    template <>
    inline void setChildParam<float>(const pugi::xml_node& node, const char* param_name, float& value) {
        if (pugi::xml_node n = node.child(param_name); n != nullptr)
            value = n.text().as_float();
    }

    template <>
    inline void setChildParam<std::string>(const pugi::xml_node& node, const char* param_name, std::string& value) {
        if (pugi::xml_node n = node.child(param_name); n != nullptr)
            value = n.text().as_string();
    }

    template <>
    inline void setChildParam<glm::vec3>(const pugi::xml_node& node, const char* param_name, glm::vec3& value) {
        if (pugi::xml_node n = node.child(param_name); n != nullptr)
            value = textToVec3(n.text().as_string());
    }

} // namespace ce
