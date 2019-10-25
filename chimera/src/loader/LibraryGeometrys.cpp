#include "LibraryGeometrys.hpp"
#include "LibraryMaterials.hpp"
#include "chimera/core/Exception.hpp"

#include <SDL2/SDL.h>

namespace ChimeraLoaders {

LibraryGeometrys::LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}

LibraryGeometrys::~LibraryGeometrys() {}

Chimera::Mesh* LibraryGeometrys::target() {

    tinyxml2::XMLElement* l_nGeo = root->FirstChildElement("library_geometries")->FirstChildElement("geometry");
    for (l_nGeo; l_nGeo; l_nGeo = l_nGeo->NextSiblingElement()) {

        std::string l_id = l_nGeo->Attribute("id");
        if (url.compare(l_id) == 0) {

            Chimera::Mesh* pMesh = new Chimera::Mesh(nullptr, l_id);
            std::string idMaterial = loadMeshCollada(l_nGeo, pMesh);

            LibraryMaterials lm(root, idMaterial);
            Chimera::Material* pMaterial = lm.target();

            pMesh->setMaterial(pMaterial);
            return pMesh;
        }
    }
    throw Chimera::Exception("Geometry nao encontrado: " + url);
}

int LibraryGeometrys::getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores) {

    const char* l_numCount = _source->FirstChildElement("float_array")->Attribute("count");
    if (l_numCount != nullptr) {

        // std::vector<float> l_array;
        const char* l_vals = _source->FirstChildElement("float_array")->GetText();
        loadArrayBtScalar(l_vals, _arrayValores);
        return _arrayValores.size();
    }

    return -1;
}

std::string LibraryGeometrys::loadMeshCollada(tinyxml2::XMLElement* _nNode, Chimera::Mesh* _pMesh) {

    std::string retorno = "";
    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement("mesh");
    if (l_nMesh != nullptr) {

        // Carrega lista de vetores
        tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement("source");
        for (l_nSource; l_nSource; l_nSource = l_nSource->NextSiblingElement("source")) {

            const char* l_id = l_nSource->Attribute("id");
            if (strstr(l_id, (char*)"-positions") != nullptr) {

                // Carrega todos os vetores ponto
                std::vector<float> lista;
                getSource(l_nSource, lista);

                for (unsigned int indice = 0; indice < lista.size(); indice += 3)
                    _pMesh->meshData.vertexList.push_back(
                        glm::vec3(lista[indice], lista[indice + 1], lista[indice + 2]));

            } else if (strstr(l_id, (char*)"-normals") != nullptr) {

                // carrega todos os vetores normal
                std::vector<float> lista;
                getSource(l_nSource, lista);

                for (unsigned int indice = 0; indice < lista.size(); indice += 3)
                    _pMesh->meshData.normalList.push_back(
                        glm::vec3(lista[indice], lista[indice + 1], lista[indice + 2]));

            } else if (strstr(l_id, (char*)"-map-0") != nullptr) {

                // carrega vetor posicao textura
                std::vector<float> lista;
                getSource(l_nSource, lista);
                for (unsigned int indice = 0; indice < lista.size(); indice += 2)
                    _pMesh->meshData.textureList.push_back(glm::vec2(lista[indice], lista[indice + 1]));
            }
        }

        // Carrega Lista de indices
        tinyxml2::XMLElement* l_nPoly = l_nMesh->FirstChildElement("polylist");
        if (l_nPoly != nullptr) {

            const char* l_count = l_nPoly->Attribute("count");
            const char* l_mat = l_nPoly->Attribute("material");

            retorno.assign(l_mat);

            tinyxml2::XMLElement* l_nInput = l_nPoly->FirstChildElement("input");

            std::vector<const char*> l_vOffset;
            std::vector<const char*> l_vSemantic;
            std::vector<const char*> l_vSource;

            for (l_nInput; l_nInput; l_nInput = l_nInput->NextSiblingElement("input")) {

                const char* l_offSet = l_nInput->Attribute("offset");
                const char* l_semantic = l_nInput->Attribute("semantic");
                const char* l_source = l_nInput->Attribute("source");

                l_vOffset.push_back(l_offSet);
                l_vSemantic.push_back(l_semantic);
                l_vSource.push_back(l_source);
            }

            tinyxml2::XMLElement* l_nP = l_nPoly->FirstChildElement("p");
            const char* l_indices = l_nP->GetText();
            std::vector<int> l_arrayIndex;

            loadArrayI(l_indices, l_arrayIndex);

            int l_numTriangles = atoi(l_count);

            for (unsigned l_contador = 0; l_contador < l_arrayIndex.size(); l_contador++) {
                int index = l_contador % l_vOffset.size();

                const char* l_offSet = l_vOffset[index];
                const char* l_semantic = l_vSemantic[index];
                const char* l_source = l_vSource[index];

                if (strstr(l_source, (char*)"-vertices") != nullptr) { // indices de vetor ponto

                    _pMesh->meshData.vertexIndex.push_back(l_arrayIndex[l_contador]);

                } else if (strstr(l_source, (char*)"-normals") != nullptr) { // indice de vetor normal

                    _pMesh->meshData.normalIndex.push_back(l_arrayIndex[l_contador]);

                } else if (strstr(l_source, (char*)"-map-0") != nullptr) { // indice de vetor posicao textura

                    _pMesh->meshData.textureIndex.push_back(l_arrayIndex[l_contador]);
                }
            }
            l_arrayIndex.clear();

            l_vOffset.clear();
            l_vSemantic.clear();
            l_vSource.clear();
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Nome: %s", _pMesh->getName().c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex  Indice / Lista - ( %03d / %03d )",
                 (int)_pMesh->meshData.vertexIndex.size(), (int)_pMesh->meshData.vertexList.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal  Indice / Lista - ( %03d / %03d )",
                 (int)_pMesh->meshData.normalIndex.size(), (int)_pMesh->meshData.normalList.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture Indice / Lista - ( %03d / %03d )",
                 (int)_pMesh->meshData.textureIndex.size(), (int)_pMesh->meshData.textureList.size());

    return retorno;
}
} // namespace ChimeraLoaders