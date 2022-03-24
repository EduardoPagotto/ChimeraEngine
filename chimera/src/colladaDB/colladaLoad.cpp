#include "chimera/colladaDB/colladaLoad.hpp"
#include "chimera/colladaDB/ColladaPhysicScene.hpp"
#include "chimera/colladaDB/ColladaVisualScene.hpp"

namespace Chimera {
void colladaLoad(Registry& r, const std::string& pathFile) {

    std::string final1 = "file://" + pathFile + "#Scene";
    std::string final2 = "file://" + pathFile + "#Scene-Physics";

    Collada cl;
    pugi::xml_node vs = cl.urlRoot(pugi::xml_node(), "library_visual_scenes", final1);

    ColladaVisualScene cn;
    cn.loadAll(vs, &r);

    pugi::xml_node fs = cl.urlRoot(pugi::xml_node(), "library_physics_scenes", final2);
    ColladaPhysicScene cs;
    cs.loadAll(fs, &r);
}
} // namespace Chimera