#include "chimera/loader/VisualScene.hpp"
#include "LibraryVisualSCenes.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

VisualScene::VisualScene(const std::string& _file, NodeGroup* _pGroup) : Library(nullptr, "!" + _file) {
    pListNodes = Singleton<ListNodes>::getRefSingleton();
    pGroup = _pGroup;
}

VisualScene::~VisualScene() { Singleton<ListNodes>::releaseRefSingleton(); }

void VisualScene::target() {
    tinyxml2::XMLElement* l_nVisual = root->FirstChildElement("scene")->FirstChildElement("instance_visual_scene");
    if (l_nVisual) {
        std::string l_url = l_nVisual->Attribute("url");
        LibraryVisualScenes lib(root, l_url, pGroup);
        lib.target();
    }
}
} // namespace Chimera