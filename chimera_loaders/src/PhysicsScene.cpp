#include "PhysicsScene.hpp"
#include "LibraryPhysicsScenes.hpp"

namespace ChimeraLoaders {

PhysicsScene::PhysicsScene(const std::string& _file,
                           Chimera::PhysicsControl* _pPhysicsControl)
    : Library(nullptr, "!" + _file) {
    pPhysicsControl = _pPhysicsControl;
}

PhysicsScene::~PhysicsScene() {}

void PhysicsScene::target() {
    tinyxml2::XMLElement* l_nPhysic =
        root->FirstChildElement("scene")->FirstChildElement("instance_physics_scene");
    if (l_nPhysic) {
        std::string l_url = l_nPhysic->Attribute("url");

        LibraryPhysicsScenes lib(root, l_url, pPhysicsControl);
        lib.target();
    }
}
} // namespace ChimeraLoaders