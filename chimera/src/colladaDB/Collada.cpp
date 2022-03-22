#include "chimera/colladaDB/Collada.hpp"
#include <SDL2/SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Chimera {

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) { // TODO: ler como matrix!!!!!!
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
        arrayI.push_back(std::stod(val));
}

const glm::vec4 textToVec4(const std::string& text) {
    std::vector<float> arrayFloat;
    textToFloatArray(text, arrayFloat);

    // return glm::vec4(l_arrayF[0], l_arrayF[1], l_arrayF[2], 1.0f);
    if (arrayFloat.size() == 4)
        return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], arrayFloat[3]);

    return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], 1.0f); // FIXME: melhorar !!!!
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

const pugi::xml_node colladaGetLibrary(const pugi::xml_node& node, const std::string& libraryName, const std::string key) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {
        std::string name = n.name();
        if (name == libraryName) {
            SDL_Log("%s encontrado", libraryName.c_str());
            for (pugi::xml_node t = n.first_child(); t; t = t.next_sibling()) {

                pugi::xml_attribute attr = t.attribute("id");

                // for (pugi::xml_attribute attr = person.first_attribute(); attr; attr = attr.next_attribute())
                // {
                //     std::cout << " " << attr.name() << "=" << attr.value() << std::endl;
                // }

                std::string id = attr.value();
                if (id == key) {
                    SDL_Log("%s item %s id %s", libraryName.c_str(), t.name(), attr.value());
                    return t;
                }
            }
        }
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s não encontrado key: %s", libraryName.c_str(), key.c_str());
    return pugi::xml_node();
}

Collada::~Collada() {
    if (doc != nullptr) {
        doc.reset();
    }
}

const pugi::xml_node Collada::urlRoot(const pugi::xml_node& nodeParent, const std::string& libraryName, const std::string& url) {

    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);
    std::size_t found = url.find(urlFile, 0, urlFileLen);

    if (found != std::string::npos) {

        std::string temp = url.substr(urlFileLen, std::string::npos); //"file://./assets/models/piso2_mestre.xml#Scene"

        std::size_t mark1 = temp.rfind("#"); // FIXME: criar erro se nao achar!!!!
        if (mark1 == std::string::npos) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "URL %s não encontrado: #", temp.c_str());
            return pugi::xml_node();
        }

        std::string key = temp.substr(mark1 + 1, std::string::npos);
        std::string target = url.substr(urlFileLen, mark1);

        pugi::xml_parse_result result = doc.load_file(target.c_str());

        if (result.status != pugi::status_ok) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Arquivo %s não encontrado", target.c_str());
            return pugi::xml_node();
        }

        SDL_Log("Novo Arquivo carregado %s, Key: %s, Status: %s", target.c_str(), key.c_str(), result.description());
        root = doc.child("COLLADA");
        return colladaGetLibrary(root, libraryName, key);

    } else {

        root = nodeParent.root().child("COLLADA");
        std::size_t found = url.rfind("#");
        std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;
        return colladaGetLibrary(root, libraryName, key);
    }
}
} // namespace Chimera