#include "chimera/core/collada/ColladaMesh.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"

namespace Chimera {

ColladaMesh::~ColladaMesh() {}

void ColladaMesh::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nMesh) {

    MeshComponent& eMesh = entity.addComponent<MeshComponent>();
    eMesh.mesh = new Mesh();
    eMesh.tag.id = id;
    eMesh.tag.tag = name;
    eMesh.tag.serial = Collada::getNewSerial();

    for (pugi::xml_node source = nMesh.first_child(); source; source = source.next_sibling()) {

        std::string name = source.name();
        std::string id = source.attribute("id").value();

        if (name == "source") {

            std::vector<float> v;
            pugi::xml_node nList = source.child("float_array");
            textToFloatArray(nList.text().as_string(), v);

            if (id.find("-positions") != std::string::npos) {

                for (size_t indice = 0; indice < v.size(); indice += 3)
                    eMesh.mesh->point.push_back(glm::vec3(v[indice], v[indice + 1], v[indice + 2]));

            } else if (id.find("-normals") != std::string::npos) {

                for (size_t indice = 0; indice < v.size(); indice += 3)
                    eMesh.mesh->normal.push_back(glm::vec3(v[indice], v[indice + 1], v[indice + 2]));

            } else if (id.find("-map-0") != std::string::npos) {

                for (size_t indice = 0; indice < v.size(); indice += 2)
                    eMesh.mesh->uv.push_back(glm::vec2(v[indice], v[indice + 1]));
            }

        } else if (name == "vertices") {
        } else if (name == "polylist") {

            std::vector<std::string> semantics;

            for (pugi::xml_node nInput = source.first_child(); nInput; nInput = nInput.next_sibling()) {
                std::string inputName = nInput.name();
                if (inputName == "input") {

                    semantics.push_back(nInput.attribute("semantic").value());

                } else if (inputName == "vcount") {
                } else if (inputName == "p") {

                    std::vector<uint32_t> arrayIndex;
                    textToUIntArray(nInput.text().as_string(), arrayIndex);

                    for (uint32_t l_contador = 0; l_contador < arrayIndex.size(); l_contador++) {

                        uint32_t index = l_contador % semantics.size();
                        const std::string& semantic = semantics[index];

                        if (semantic == "VERTEX") {
                            eMesh.mesh->iPoint.push_back(arrayIndex[l_contador]);
                        } else if (semantic == "NORMAL") {
                            eMesh.mesh->iNormal.push_back(arrayIndex[l_contador]);
                        } else if (semantic == "TEXCOORD") {
                            eMesh.mesh->iUv.push_back(arrayIndex[l_contador]);
                        }
                    }
                    arrayIndex.clear();
                }
            }
            semantics.clear();
        }
    }
}
} // namespace Chimera