#include "chimera/core/Exception.hpp"

#include "LibraryVisualSCenes.hpp"
#include "chimera/loader/VisualScene.hpp"

namespace ChimeraLoaders {

VisualScene::VisualScene(const std::string& _file, Chimera::NodeGroup* _pGroup) : Library(nullptr, "!" + _file) {
    pListNodes = Chimera::Singleton<ListNodes>::getRefSingleton();
    pGroup = _pGroup;
}

VisualScene::~VisualScene() { Chimera::Singleton<ListNodes>::releaseRefSingleton(); }

void VisualScene::target() {
    tinyxml2::XMLElement* l_nVisual = root->FirstChildElement("scene")->FirstChildElement("instance_visual_scene");
    if (l_nVisual) {
        std::string l_url = l_nVisual->Attribute("url");
        LibraryVisualScenes lib(root, l_url, pGroup);
        lib.target();
    }
}
} // namespace ChimeraLoaders