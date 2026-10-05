#include "chimera_collada/ColladaGeometry.hpp"
#include "chimera_collada/ColladaCube.hpp"
#include "chimera_collada/ColladaHeightMap.hpp"
#include "chimera_collada/ColladaMesh.hpp"
#include "chimera_collada/ColladaParticle.hpp"
#include "chimera_collada/ColladaWaveFront.hpp"

namespace ce {
    void ColladaGeometry::create(Entity& entity, pugi::xml_node geo) {

        std::string id = geo.attribute("id").value();
        std::string name = geo.attribute("name").value();

        if (pugi::xml_node mesh = geo.child("mesh"); mesh != nullptr) {
            ColladaMesh cf(registry, colladaDom, "#vazio");
            cf.create(id, name, entity, mesh);

        } else {
            const pugi::xml_node n_extra = geo.child("extra");
            if (const pugi::xml_node n_obj = getExtra(n_extra, "external_obj"); n_obj != nullptr) {
                ColladaWaveFront cf(registry, colladaDom, "#vazio");
                cf.create(id, name, entity, n_obj);
            }

            if (const pugi::xml_node n_cube = getExtra(n_extra, "external_cube"); n_cube) {
                ColladaCube cc(registry, colladaDom, "#vazio");
                cc.create(id, name, entity, n_cube);
            }

            if (const pugi::xml_node n_height = getExtra(n_extra, "external_height"); n_height) {
                ColladaHeightMap ch(registry, colladaDom, "#vazio");
                ch.create(id, name, entity, n_height);
            }

            if (const pugi::xml_node n_particle = getExtra(n_extra, "particle"); n_particle) {
                ColladaParticle cp(registry, colladaDom, "#vazio");
                cp.create(id, name, entity, n_particle);
            }
        }
    }
} // namespace ce
