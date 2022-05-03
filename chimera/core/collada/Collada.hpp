#pragma once
#include "ColladaDom.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class Collada {
  public:
    Collada(ColladaDom& dom, const std::string& url);
    virtual ~Collada();
    const pugi::xml_node getLibrary(const std::string& libraryName, const std::string& url);
    static uint32_t getNewSerial() { return ++serial; }
    static void destroy();

    static std::vector<ColladaDom> vColladaDom;

  protected:
    std::string urlLocal;
    ColladaDom colladaDom;
    static uint32_t serial;

  private:
    static bool isLocalURL(const std::string& url);
    static ColladaDom urlLib(const std::string& url);
};

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter);
void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat);
void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI);
const glm::vec4 textToVec4(const std::string& text);
const glm::mat4 textToMat4(const std::string& text);
const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name);
} // namespace Chimera