#pragma once
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Chimera {

class Collada {
  public:
    Collada() = default;
    virtual ~Collada();
    const pugi::xml_node urlRoot(const pugi::xml_node& nodeParent, const std::string& libraryName, const std::string& url);

    static uint32_t getNewSerial() { return ++serial; }

  protected:
    pugi::xml_document doc;
    pugi::xml_node root;

    static uint32_t serial;
};

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter);
void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat);
void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI);
const glm::vec4 textToVec4(const std::string& text);
const glm::mat4 textToMat4(const std::string& text);

const pugi::xml_node colladaGetLibrary(const pugi::xml_node& node, const std::string& libraryName, const std::string key);
} // namespace Chimera