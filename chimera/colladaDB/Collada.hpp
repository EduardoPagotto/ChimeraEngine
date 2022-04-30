#pragma once
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Chimera {

struct ColladaDom {
    ColladaDom() = default;
    ColladaDom(const ColladaDom& other) = default;
    std::string file = "";
    pugi::xml_node root = pugi::xml_node();
    pugi::xml_document* pDoc = nullptr;
};

class Collada {
  public:
    Collada(ColladaDom& dom, const std::string& url);
    virtual ~Collada();
    const pugi::xml_node getLibrary(const std::string& libraryName, const std::string& url);

    static bool isLocalURL(const std::string& url);
    static ColladaDom urlLib(const std::string& url);
    static uint32_t getNewSerial() { return ++serial; }
    static std::vector<ColladaDom> vColladaDom;

  protected:
    std::string urlLocal;
    ColladaDom colladaDom;
    static uint32_t serial;
};

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter);
void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat);
void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI);
const glm::vec4 textToVec4(const std::string& text);
const glm::mat4 textToMat4(const std::string& text);
} // namespace Chimera