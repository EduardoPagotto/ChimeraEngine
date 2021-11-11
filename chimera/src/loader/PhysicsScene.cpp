#include "chimera/loader/PhysicsScene.hpp"
#include "LibraryPhysicsScenes.hpp"

namespace Chimera {

PhysicsScene::PhysicsScene(const std::string& _file, PhysicsControl* _pPhysicsControl, Scene* scene) : Library(nullptr, "!" + _file) {
    pPhysicsControl = _pPhysicsControl;
    this->scene = scene;
}

PhysicsScene::~PhysicsScene() {}

void PhysicsScene::target() {
    tinyxml2::XMLElement* l_nPhysic = root->FirstChildElement("scene")->FirstChildElement("instance_physics_scene");
    if (l_nPhysic) {
        std::string l_url = l_nPhysic->Attribute("url");

        LibraryPhysicsScenes lib(root, l_url, pPhysicsControl, scene);
        lib.target();
    }
}
} // namespace Chimera