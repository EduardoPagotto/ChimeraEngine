#include "chimera/core/collada/ColladaGeometry.hpp"
#include "chimera/core/collada/ColladaCube.hpp"
#include "chimera/core/collada/ColladaHeightMap.hpp"
#include "chimera/core/collada/ColladaWaveFront.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"

namespace Chimera {

ColladaGeometry::~ColladaGeometry() {}

void ColladaGeometry::create(Entity& entity, pugi::xml_node geo) {
    pugi::xml_node mesh = geo.child("mesh");

    std::string id = geo.attribute("id").value();
    std::string name = geo.attribute("name").value();

    if (mesh != nullptr) {

        MeshComponent& eMesh = entity.addComponent<MeshComponent>();
        eMesh.mesh = new Mesh();
        eMesh.tag.id = id;
        eMesh.tag.tag = name;
        eMesh.tag.serial = Collada::getNewSerial();

        for (pugi::xml_node source = mesh.first_child(); source; source = source.next_sibling()) {

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
    } else {
        const pugi::xml_node nExtra = geo.child("extra");
        const pugi::xml_node nObj = getExtra(nExtra, "external_obj");
        if (nObj != nullptr) {
            ColladaWaveFront cf(colladaDom, "#vazio");
            cf.create(id, name, entity, nObj);
        }

        const pugi::xml_node nCube = getExtra(nExtra, "external_cube");
        if (nCube) {
            ColladaCube cc(colladaDom, "#vazio");
            cc.create(id, name, entity, nCube);
        }

        const pugi::xml_node nHeight = getExtra(nExtra, "external_height");
        if (nHeight) {
            ColladaHeightMap ch(colladaDom, "#vazio");
            ch.create(id, name, entity, nHeight);
        }

        const pugi::xml_node nParticle = getExtra(nExtra, "particle");
        if (nParticle) {

            // loadMaterial(entity, "#vazio", nParticle);
            glm::vec3 dir = textToVec3(nParticle.child("emmiter_font").child("maindir").text().as_string());
            float spread = nParticle.child("emmiter_font").child("spread").text().as_float();

            EmitterComponent& ec = entity.addComponent<EmitterComponent>();
            ec.tag.id = id;
            ec.tag.tag = name;
            ec.emitter = new EmitterFont(dir, spread); // EF to R

            ParticleContainer* pc = new ParticleContainer();
            pc->life = nParticle.child("container").child("life").text().as_float();
            pc->max = nParticle.child("container").child("max").text().as_int();
            pc->respaw = nParticle.child("container").child("respaw").text().as_bool();
            ec.emitter->pushParticleContainer(pc);
        }
    }
}
} // namespace Chimera