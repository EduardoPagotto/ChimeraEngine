#pragma once
#include "ColladaDom.hpp"
#include "RFC3986.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class Collada {
  public:
    Collada(ColladaDom& dom, const std::string& url);
    virtual ~Collada();
    const pugi::xml_node getLibraryUrl(const std::string& libraryName, const std::string& url);
    const pugi::xml_node getLibrary(const std::string& libraryName);
    static uint32_t getNewSerial() { return ++serial; }
    static void destroy();
    static std::vector<ColladaDom> vColladaDom;

  protected:
    ColladaDom colladaDom;
    static uint32_t serial;
    std::string fragment;

  private:
    const pugi::xml_node getLibraryKey(const std::string& libraryName, const std::string& key);
};

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter);
void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat);
void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI);
const glm::vec4 textToVec4(const std::string& text);
const glm::mat4 textToMat4(const std::string& text);
const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name);
} // namespace Chimera