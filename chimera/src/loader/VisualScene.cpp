#include "chimera/loader/VisualScene.hpp"
#include "LibraryVisualSCenes.hpp"

namespace Chimera {

void VisualScene::target() {
    tinyxml2::XMLElement* l_nVisual = root->FirstChildElement("scene")->FirstChildElement("instance_visual_scene");
    if (l_nVisual) {
        std::string l_url = l_nVisual->Attribute("url");
        LibraryVisualScenes lib(root, l_url, reg);
        lib.target();
    }
}
} // namespace Chimera