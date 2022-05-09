#include "chimera/core/collada/ColladaVisualScene.hpp"
#include "chimera/core/collada/ColladaCam.hpp"
#include "chimera/core/collada/ColladaGeometry.hpp"
#include "chimera/core/collada/ColladaLight.hpp"
#include "chimera/core/collada/ColladaMaterial.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"
#include "chimera/core/visible/Transform.hpp"

namespace Chimera {

ColladaVisualScene::~ColladaVisualScene() {}

void ColladaVisualScene::loadNode(pugi::xml_node node, Registry* reg) {

    std::string entName = node.attribute("name").value();
    std::string entId = node.attribute("id").value();
    Entity entity = reg->createEntity(entName, entId);
    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
        nodeData(n, entity);
}

void ColladaVisualScene::nodeData(pugi::xml_node n, Entity entity) {

    std::string name = n.name();
    std::string url = n.attribute("url").value();
    if (name == "matrix") {

        std::string sid = n.attribute("sid").value();
        if (sid == "transform") {
            TransComponent& tc = entity.addComponent<TransComponent>();
            tc.trans = new Transform(textToMat4(n.text().as_string()));
        }

    } else if (name == "instance_geometry") {

        ColladaGeometry cg(colladaDom, url);
        cg.create(entity, cg.getLibrary("library_geometries"));

        pugi::xml_node instanceMaterial = n.child("bind_material").child("technique_common").child("instance_material");
        if (instanceMaterial != nullptr) {
            ColladaMaterial cm(colladaDom, url);
            cm.create(entity, instanceMaterial);
        }

    } else if (name == "instance_light") {

        ColladaLight cl(colladaDom, url);
        cl.create(entity, cl.getLibrary("library_lights"));

    } else if (name == "instance_node") {

        ColladaVisualScene vs(colladaDom, url);
        const pugi::xml_node nNodes = vs.getLibrary("library_nodes");
        for (pugi::xml_node n = nNodes.first_child(); n; n = n.next_sibling()) {
            vs.nodeData(n, entity);
        }

    } else if (name == "instance_camera") {

        ColladaCam cc(colladaDom, url);
        cc.create(entity, cc.getLibrary("library_cameras"));
    } else if (name == "node") {
        // TODO: implementar hierarquia
    } else if (name == "extra") {

        const pugi::xml_node nParticle = getExtra(n, "particle");
        if (nParticle) {

            pugi::xml_node instanceMaterial = nParticle.child("bind_material").child("technique_common").child("instance_material");
            if (instanceMaterial != nullptr) {
                ColladaMaterial cm(colladaDom, "#vazio");
                cm.create(entity, instanceMaterial);
            }

            glm::vec3 dir = textToVec3(nParticle.child("emmiter_font").child("maindir").text().as_string());
            float spread = nParticle.child("emmiter_font").child("spread").text().as_float();

            EmitterFont* ef = new EmitterFont(dir, spread); // EF to R

            ParticleContainer& pc = entity.addComponent<ParticleContainer>();
            pc.life = nParticle.child("container").child("life").text().as_float();
            pc.max = nParticle.child("container").child("max").text().as_int();
            pc.respaw = nParticle.child("container").child("respaw").text().as_bool();

            ef->pushParticleContainer(&pc);
        }
    }
}

void ColladaVisualScene::loadAll(pugi::xml_node node, Registry* reg) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling())
        loadNode(n, reg);
}

} // namespace Chimera