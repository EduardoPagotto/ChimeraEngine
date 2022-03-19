#include "chimera/colladaDB/loader.hpp"
#include "SDL2/SDL.h"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Chimera {

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

InstanceCollada* colladaURL(InstanceCollada* handle, const std::string& libraryName, const std::string& url) {

    const char* urlFile = "file://";
    size_t urlFileLen = strlen(urlFile);
    std::size_t found = url.find(urlFile, 0, urlFileLen);

    if (found != std::string::npos) {

        std::string temp = url.substr(urlFileLen, std::string::npos); //"file://./assets/models/piso2_mestre.xml#Scene"

        std::size_t mark1 = temp.rfind("#"); // FIXME: criar erro se nao achar!!!!
        if (mark1 == std::string::npos) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "URL %s não encontrado: #", temp.c_str());
            return nullptr;
        }

        std::string key = temp.substr(mark1 + 1, std::string::npos);
        std::string target = url.substr(urlFileLen, mark1);
        InstanceCollada* ret = new InstanceCollada;

        pugi::xml_parse_result result = ret->doc.load_file(target.c_str());

        if (result.status != pugi::status_ok) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Arquivo %s não encontrado", target.c_str());
            return nullptr;
        }

        SDL_Log("Novo Arquivo carregado %s, Key: %s, Status: %s", target.c_str(), key.c_str(), result.description());
        ret->root = ret->doc.child("COLLADA");
        ret->node = colladaGetLibrary(ret->root, libraryName, key);
        return ret;

    } else {

        std::size_t found = url.rfind("#");
        std::string key = (found != std::string::npos) ? url.substr(found + 1, std::string::npos) : url;
        handle->node = colladaGetLibrary(handle->root, libraryName, key);

        return nullptr;
    }
}

Texture* loadImage(InstanceCollada* handle, const std::string& id) {

    InstanceCollada* i = colladaURL(handle, "library_images", id);
    pugi::xml_node node = (i == nullptr) ? handle->node : i->node;
    pugi::xml_node init = node.child("init_from");
    if (init != nullptr) {
        pugi::xml_text pathFile = init.text();
        if (pathFile != nullptr) {
            std::string f = pathFile.as_string();
            SDL_Log("Nova textura %s, Key: %s", f.c_str(), id.c_str());

            TextureManager::loadFromFile(id, f, TexParam());
            return TextureManager::getLast();
        }
    }

    return nullptr;
}
//---
void textSplit(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) { // TODO: ler como matrix!!!!!!
    std::string token;
    std::istringstream tokenStream(sIn);
    while (std::getline(tokenStream, token, delimiter))
        vOut.push_back(token);
}

void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat) {
    std::vector<std::string> textData;
    textSplit(text, textData, ' ');
    for (const std::string& val : textData)
        arrayFloat.push_back(std::stod(val));
}

glm::mat4 floatArrayToMat4(const std::vector<float>& listaMatrix) {

    if (listaMatrix.size() != 16)
        throw std::string("Tamanho da Matrix invalido" + std::to_string(listaMatrix.size()));

    float ponteiroFloat[16];
    int indice = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int pos = i + (4 * j);
            ponteiroFloat[pos] = listaMatrix[indice];
            indice++;
        }
    }

    return glm::make_mat4(&ponteiroFloat[0]);
}

void loadNode(InstanceCollada* handle, Registry* reg) {

    Entity entity = reg->createEntity(handle->node.attribute("name").value(), handle->node.attribute("id").value());
    for (pugi::xml_node n = handle->node.first_child(); n; n = n.next_sibling()) {

        std::string val = n.name();
        if (val == "matrix") {
            std::string sid = n.attribute("sid").value();
            if (sid == "transform") {

                std::vector<float> arrayFloat;
                textToFloatArray(n.text().as_string(), arrayFloat);
                glm::mat4 transLocal = floatArrayToMat4(arrayFloat);

                ComponentTrans tc = entity.addComponent<ComponentTrans>(new Transform(transLocal));
                // tc.trans = new Transform(transLocal);
            }
        } else if (val == "instance_geometry") {

            pugi::xml_node nodeGeo = n;
            // InstanceCollada* novo = colladaURL(handle, "library_geometries", nodeGeo.attribute("url").value());
            // if (novo != nullptr)
            //     handle = novo;

            pugi::xml_node instanceMaterial = nodeGeo.child("bind_material").child("technique_common").child("instance_material");
            handle->node = instanceMaterial;
            InstanceCollada* novo = colladaURL(handle, "library_materials", instanceMaterial.attribute("symbol").value());
            if (novo != nullptr)
                handle = novo;

            Material& eMaterial = entity.addComponent<Material>();

            pugi::xml_node nodeMat = handle->node;
            pugi::xml_node effect = nodeMat.child("instance_effect");

            InstanceCollada* novo2 = colladaURL(handle, "library_effects", effect.attribute("url").value());
            if (novo2 != nullptr)
                handle = novo2;

            pugi::xml_node profile = handle->node.first_child();

            std::unordered_map<std::string, Texture*> mapaTex;
            std::unordered_map<std::string, std::string> mapa2D;

            // pugi::xml_node profile = handle->node;
            for (pugi::xml_node param = profile.first_child(); param; param = param.next_sibling()) {

                std::string sProf = param.name();
                std::string sid = param.attribute("sid").value();
                if (sProf == "newparam") {

                    pugi::xml_node val1 = param.first_child();
                    std::string sVal1 = val1.name();

                    if (sVal1 == "surface") {

                        std::string keyImage = val1.child("init_from").text().as_string();
                        Texture* tex = loadImage(handle, keyImage);
                        mapaTex[sid] = tex;

                    } else if (sVal1 == "sampler2D") {

                        std::string keyMap = val1.child("source").text().as_string();
                        mapa2D[sid] = keyMap;
                    }

                } else if (sProf == "technique") {

                    pugi::xml_node phong = param.child("phong");
                    for (pugi::xml_node prop = phong.first_child(); prop; prop = prop.next_sibling()) {

                        std::string p = prop.name();
                        if (p == "emission") {
                        } else if (p == "ambient") {
                        } else if (p == "diffuse") {
                        } else if (p == "specular") {
                        } else if (p == "shininess") {
                        } else if (p == "index_of_refraction") {
                        }
                    }
                }
            }
        }
    }
}

void loadAll(InstanceCollada* handle, Registry* reg) {

    for (pugi::xml_node n = handle->node.first_child(); n; n = n.next_sibling()) {
        handle->node = n;
        loadNode(handle, reg);
    }
}

} // namespace Chimera