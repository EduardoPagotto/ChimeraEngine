#pragma once
#include "ColladaDom.hpp"
#include "RFC3986.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string.h>
#include <vector>

namespace Chimera {

class Collada {
  protected:
    ColladaDom m_colladaDom;

  public:
    Collada(ColladaDom& dom, const std::string& url) {

        RFC3986 rfc(url);
        if (rfc.isInvalid() == true)
            throw std::string("URL " + url + " invalida");

        if (rfc.getScheme() == RFC3986_SCHEME::LOCAL)
            m_colladaDom = dom;
        else {

            for (auto domCache : Collada::vColladaDom) {
                if (domCache.file == rfc.getPath()) {
                    m_colladaDom = domCache;
                    m_fragment = rfc.getFragment();
                    SDL_Log("Arquivo %s cache, id: %s", m_colladaDom.file.c_str(), rfc.getFragment().c_str());
                    return;
                }
            }

            m_colladaDom.file = rfc.getPath();
            m_colladaDom.pDoc = new pugi::xml_document();
            pugi::xml_parse_result result = m_colladaDom.pDoc->load_file(m_colladaDom.file.c_str());
            if (result.status != pugi::status_ok)
                throw std::string("Arquivo " + m_colladaDom.file + " erro: %s" + std::string(result.description()));

            SDL_Log("Arquivo %s novo, id: %s Status: %s", m_colladaDom.file.c_str(), rfc.getFragment().c_str(), result.description());
            m_colladaDom.root = m_colladaDom.pDoc->child("COLLADA");

            Collada::vColladaDom.push_back(m_colladaDom);
        }

        m_fragment = rfc.getFragment();
    }

    virtual ~Collada() = default;

    const pugi::xml_node getLibraryUrl(const std::string& libraryName, const std::string& url) {
        std::size_t found = url.find('#');
        std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;
        return getLibraryKey(libraryName, key);
    }

    const pugi::xml_node getLibrary(const std::string& libraryName) { return getLibraryKey(libraryName, m_fragment); }

    static uint32_t getNewSerial() { return ++s_serial; }

    static void destroy() {

        for (ColladaDom dom : vColladaDom)
            dom.pDoc->reset();

        while (vColladaDom.size() != 0) {
            std::vector<ColladaDom>::iterator it = vColladaDom.begin();
            delete (*it).pDoc;
            (*it).pDoc = nullptr;

            vColladaDom.erase(it);
        }
    }

    inline static std::vector<ColladaDom> vColladaDom;

  private:
    std::string m_fragment;
    inline static uint32_t s_serial;

    const pugi::xml_node getLibraryKey(const std::string& libraryName, const std::string& key) {

        for (pugi::xml_node n = m_colladaDom.root.first_child(); n; n = n.next_sibling()) {
            std::string name = n.name();
            if (name == libraryName) {

                for (pugi::xml_node t = n.first_child(); t; t = t.next_sibling()) {
                    if (std::string id = t.attribute("id").value(); id == key) {
                        SDL_Log("%s: %s id: %s", libraryName.c_str(), t.name(), id.c_str());
                        return t;
                    }
                }
            }
        }

        throw std::string(libraryName + " não encontrado id: " + key);
    }
};

inline void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) {
    std::string token;
    std::istringstream tokenStream(sIn);
    while (std::getline(tokenStream, token, delimiter))
        vOut.push_back(token);
}

inline void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData) {
        if (val.size() != 0)
            arrayFloat.push_back(std::stod(val));
    }
}

inline void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData)
        arrayI.push_back(static_cast<uint32_t>(std::stoul(val)));
}

inline const glm::vec3 textToVec3(const std::string& text) {
    std::vector<float> arrayFloat;
    textToFloatArray(text, arrayFloat);

    return glm::vec3(arrayFloat[0], arrayFloat[1], arrayFloat[2]);
}

inline const glm::vec4 textToVec4(const std::string& text) {
    std::vector<float> arrayFloat;
    textToFloatArray(text, arrayFloat);
    if (arrayFloat.size() == 4)
        return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], arrayFloat[3]);

    return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], 1.0f);
}

inline const glm::mat4 textToMat4(const std::string& text) {

    std::vector<float> arrayFloat;
    textToFloatArray(text, arrayFloat);

    if (arrayFloat.size() != 16)
        throw std::string("Tamanho da Matrix invalido" + std::to_string(arrayFloat.size()));

    float ponteiroFloat[16];
    int indice = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int pos = i + (4 * j);
            ponteiroFloat[pos] = arrayFloat[indice];
            indice++;
        }
    }

    return glm::make_mat4(&ponteiroFloat[0]);
}

inline const pugi::xml_node getExtra(const pugi::xml_node node, const std::string& name) {
    for (pugi::xml_node nTec = node.first_child(); nTec; nTec = nTec.next_sibling()) {
        if ((strcmp(nTec.name(), "technique") == 0) and (strcmp(nTec.attribute("profile").value(), "chimera") == 0))
            return nTec.child(name.c_str());
    }

    return pugi::xml_node();
}

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

} // namespace Chimera