#include "chimera/core/collada/ColladaGeometry.hpp"
#include "chimera/core/collada/ColladaCube.hpp"
#include "chimera/core/collada/ColladaHeightMap.hpp"
#include "chimera/core/collada/ColladaMesh.hpp"
#include "chimera/core/collada/ColladaParticle.hpp"
#include "chimera/core/collada/ColladaWaveFront.hpp"

namespace Chimera {

ColladaGeometry::~ColladaGeometry() {}

void ColladaGeometry::create(Entity& entity, pugi::xml_node geo) {
    pugi::xml_node mesh = geo.child("mesh");

    std::string id = geo.attribute("id").value();
    std::string name = geo.attribute("name").value();

    if (mesh != nullptr) {
        ColladaMesh cf(colladaDom, "#vazio");
        cf.create(id, name, entity, mesh);
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

            ColladaParticle cp(colladaDom, "#vazio");
            cp.create(id, name, entity, nParticle);
        }
    }
}
} // namespace Chimera