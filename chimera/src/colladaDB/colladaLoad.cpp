#include "chimera/colladaDB/colladaLoad.hpp"
#include "chimera/colladaDB/ColladaPhysicScene.hpp"
#include "chimera/colladaDB/ColladaVisualScene.hpp"

namespace Chimera {
void colladaLoad(Registry& r, const std::string& pathFile) {

    ColladaVisualScene cn;
    pugi::xml_node vs = cn.loadScene(pathFile);
    for (pugi::xml_node n = vs.first_child(); n; n = n.next_sibling()) {

        std::string instance = n.name();
        std::string url = n.attribute("url").value();
        if (instance == "instance_visual_scene") {

            pugi::xml_node nodeScene = cn.urlRoot(vs, "library_visual_scenes", url);
            cn.loadAll(nodeScene, &r);

        } else if (instance == "instance_physics_scene") {

            pugi::xml_node fs = cn.urlRoot(vs, "library_physics_scenes", url);
            ColladaPhysicScene cs;
            cs.loadAll(fs, &r);
        }
    }
}
} // namespace Chimera