#include "LibraryGeometrys.hpp"
#include "LibraryMaterials.hpp"
#include "chimera/core/visible/Components.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

void LibraryGeometrys::target() {

    for (tinyxml2::XMLElement* l_nGeo = root->FirstChildElement("library_geometries")->FirstChildElement("geometry"); l_nGeo != nullptr;
         l_nGeo = l_nGeo->NextSiblingElement()) {

        std::string l_id = l_nGeo->Attribute("id");
        if (url.compare(l_id) == 0) {

            MeshComponent& eMesh = entity.addComponent<MeshComponent>();
            auto& tag = entity.getComponent<TagComponent>();
            tag.tag = l_id;
            std::string idMaterial = loadMeshCollada(l_nGeo, eMesh.mesh);

            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Nome: %s", l_id.c_str());
            // eMesh.debugDados(false);

            LibraryMaterials lm(root, idMaterial, entity);
            lm.target();

            return;
        }
    }
    throw std::string("Geometry nao encontrado: " + url);
}

int LibraryGeometrys::getSource(tinyxml2::XMLElement* _source, std::vector<float>& _arrayValores) {

    const char* l_numCount = _source->FirstChildElement("float_array")->Attribute("count");
    if (l_numCount != nullptr) {
        const char* l_vals = _source->FirstChildElement("float_array")->GetText();
        loadArrayBtScalar(l_vals, _arrayValores);
        return _arrayValores.size();
    }

    return -1;
}

std::string LibraryGeometrys::loadMeshCollada(tinyxml2::XMLElement* _nNode, Mesh* meshData) {

    std::string retorno = "";
    tinyxml2::XMLElement* l_nMesh = _nNode->FirstChildElement("mesh");
    if (l_nMesh != nullptr) {
        meshData->singleIndex = false;
        // Carrega lista de vetores
        for (tinyxml2::XMLElement* l_nSource = l_nMesh->FirstChildElement("source"); l_nSource;
             l_nSource = l_nSource->NextSiblingElement("source")) {

            const char* l_id = l_nSource->Attribute("id");
            if (strstr(l_id, (char*)"-positions") != nullptr) {

                // Carrega todos os vetores ponto
                std::vector<float> lista;
                getSource(l_nSource, lista);

                for (unsigned int indice = 0; indice < lista.size(); indice += 3)
                    meshData->point.push_back(glm::vec3(lista[indice], lista[indice + 1], lista[indice + 2]));

            } else if (strstr(l_id, (char*)"-normals") != nullptr) {

                // carrega todos os vetores normal
                std::vector<float> lista;
                getSource(l_nSource, lista);

                for (unsigned int indice = 0; indice < lista.size(); indice += 3)
                    meshData->normal.push_back(glm::vec3(lista[indice], lista[indice + 1], lista[indice + 2]));

            } else if (strstr(l_id, (char*)"-map-0") != nullptr) {

                // carrega vetor posicao textura
                std::vector<float> lista;
                getSource(l_nSource, lista);
                for (unsigned int indice = 0; indice < lista.size(); indice += 2)
                    meshData->uv.push_back(glm::vec2(lista[indice], lista[indice + 1]));
            }
        }

        // Carrega Lista de indices
        tinyxml2::XMLElement* l_nPoly = l_nMesh->FirstChildElement("polylist");
        if (l_nPoly != nullptr) {

            const char* l_count = l_nPoly->Attribute("count");
            const char* l_mat = l_nPoly->Attribute("material");

            retorno.assign(l_mat);

            std::vector<const char*> l_vOffset;
            std::vector<const char*> l_vSemantic;
            std::vector<const char*> l_vSource;

            for (tinyxml2::XMLElement* l_nInput = l_nPoly->FirstChildElement("input"); l_nInput;
                 l_nInput = l_nInput->NextSiblingElement("input")) {

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

                    meshData->iPoint.push_back(l_arrayIndex[l_contador]);

                } else if (strstr(l_source, (char*)"-normals") != nullptr) { // indice de vetor normal

                    meshData->iNormal.push_back(l_arrayIndex[l_contador]);

                } else if (strstr(l_source, (char*)"-map-0") != nullptr) { // indice de vetor posicao textura

                    meshData->iUv.push_back(l_arrayIndex[l_contador]);
                }
            }
            l_arrayIndex.clear();

            l_vOffset.clear();
            l_vSemantic.clear();
            l_vSource.clear();
        }
    }
    return retorno;
}
} // namespace Chimera