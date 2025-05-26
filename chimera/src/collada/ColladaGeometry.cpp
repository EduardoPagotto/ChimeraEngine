#include "chimera/collada/ColladaGeometry.hpp"
#include "chimera/collada/ColladaCube.hpp"
#include "chimera/collada/ColladaHeightMap.hpp"
#include "chimera/collada/ColladaMesh.hpp"
#include "chimera/collada/ColladaParticle.hpp"
#include "chimera/collada/ColladaWaveFront.hpp"

namespace ce {
void ColladaGeometry::create(Entity& entity, pugi::xml_node geo) {

    std::string id = geo.attribute("id").value();
    std::string name = geo.attribute("name").value();

    if (pugi::xml_node mesh = geo.child("mesh"); mesh != nullptr) {
        ColladaMesh cf(colladaDom, "#vazio");
        cf.create(id, name, entity, mesh);

    } else {
        const pugi::xml_node nExtra = geo.child("extra");
        if (const pugi::xml_node nObj = getExtra(nExtra, "external_obj"); nObj != nullptr) {
            ColladaWaveFront cf(colladaDom, "#vazio");
            cf.create(id, name, entity, nObj);
        }

        if (const pugi::xml_node nCube = getExtra(nExtra, "external_cube"); nCube) {
            ColladaCube cc(colladaDom, "#vazio");
            cc.create(id, name, entity, nCube);
        }

        if (const pugi::xml_node nHeight = getExtra(nExtra, "external_height"); nHeight) {
            ColladaHeightMap ch(colladaDom, "#vazio");
            ch.create(id, name, entity, nHeight);
        }

        if (const pugi::xml_node nParticle = getExtra(nExtra, "particle"); nParticle) {
            ColladaParticle cp(colladaDom, "#vazio");
            cp.create(id, name, entity, nParticle);
        }
    }
}
} // namespace ce