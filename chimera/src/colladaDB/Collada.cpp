#include "chimera/colladaDB/Collada.hpp"
#include <SDL2/SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Chimera {

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) {
    std::string token;
    std::istringstream tokenStream(sIn);
    while (std::getline(tokenStream, token, delimiter))
        vOut.push_back(token);
}

void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData)
        arrayFloat.push_back(std::stod(val));
}

void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData)
        arrayI.push_back(static_cast<uint32_t>(std::stoul(val)));
}

const glm::vec4 textToVec4(const std::string& text) {
    std::vector<float> arrayFloat;
    textToFloatArray(text, arrayFloat);
    if (arrayFloat.size() == 4)
        return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], arrayFloat[3]);

    return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], 1.0f);
}

const glm::mat4 textToMat4(const std::string& text) {

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

//--

uint32_t Collada::serial = 0;
std::vector<ColladaDom> Collada::vColladaDom;

Collada::Collada(ColladaDom& dom, const std::string& url) {

    if (Collada::isLocalURL(url) == true)
        colladaDom = dom;
    else
        colladaDom = Collada::urlLib(url);
}

Collada::~Collada() {
    // FIXME: ver como fazer
    // if (doc != nullptr) {
    //     doc.reset();
    // }
}

const pugi::xml_node Collada::getLibrary(const std::string& libraryName, const std::string& url) {

    std::size_t found = url.find("#");
    std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;

    for (pugi::xml_node n = colladaDom.root.first_child(); n; n = n.next_sibling()) {
        std::string name = n.name();
        if (name == libraryName) {

            for (pugi::xml_node t = n.first_child(); t; t = t.next_sibling()) {

                pugi::xml_attribute attr = t.attribute("id");
                std::string id = attr.value();
                if (id == key) {
                    SDL_Log("%s: %s url: %s", libraryName.c_str(), t.name(), attr.value());
                    return t;
                }
            }
        }
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s não encontrado url: %s", libraryName.c_str(), key.c_str());
    throw std::string("Falha na localização da biblioteca");
}

bool Collada::isLocalURL(const std::string& url) {
    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);
    std::size_t found = url.find(urlFile, 0, urlFileLen);
    if (found != std::string::npos)
        return false;

    return true;
}

ColladaDom Collada::urlLib(const std::string& url) {
    ColladaDom dom;

    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);
    std::size_t found = url.find(urlFile, 0, urlFileLen);

    if (found != std::string::npos) {

        std::string temp = url.substr(urlFileLen, std::string::npos); //"file://./assets/models/piso2_mestre.xml#Scene"
        std::size_t mark1 = temp.rfind("#");
        if (mark1 == std::string::npos) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "URL %s não encontrado: ", url.c_str());
            throw std::string("Falha na carga do arquivo");
        }

        std::string key = temp.substr(mark1 + 1, std::string::npos);

        std::string target = url.substr(urlFileLen, mark1);
        for (auto domCache : Collada::vColladaDom) {
            if (domCache.file == target)
                return domCache;
        }

        dom.file = url.substr(urlFileLen, mark1);
        dom.pDoc = new pugi::xml_document();

        pugi::xml_parse_result result = dom.pDoc->load_file(dom.file.c_str());
        if (result.status != pugi::status_ok) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Arquivo %s não encontrado", dom.file.c_str());
            throw std::string("Falha na carga do arquivo");
        }

        SDL_Log("Novo Arquivo: %s url: %s Status: %s", dom.file.c_str(), key.c_str(), result.description());
        dom.root = dom.pDoc->child("COLLADA");
        Collada::vColladaDom.push_back(dom);
    }

    return dom;
}
} // namespace Chimera