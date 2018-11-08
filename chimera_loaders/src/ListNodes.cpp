#include "ListNodes.h"

namespace ChimeraLoaders {

ListNodes::ListNodes() noexcept {
}

void ListNodes::reset() noexcept {
    vMesh.clear();
    vLight.clear();
    vCamera.clear();
}

Chimera::Node* ListNodes::getByName(const Chimera::EntityKind &_kind, const std::string &_name) const {

    switch(_kind){
        case Chimera::EntityKind::CAMERA:
            for (std::vector<Chimera::Node*>::iterator item; item != vCamera.end(); item++)
                if ((*item)->getName().compare(_name)==0)
                    return (*item);
            break;

        case Chimera::EntityKind::MESH:
            for (std::vector<Chimera::Node*>::iterator item; item != vMesh.end(); item++)
                if ((*item)->getName().compare(_name)==0)
                    return (*item);
            break;

        case Chimera::EntityKind::LIGHT:
            for (std::vector<Chimera::Node*>::iterator item; item != vLight.end(); item++)
                if ((*item)->getName().compare(_name)==0)
                    return (*item);
            break; 
    }

    return nullptr;
}

void ListNodes::addNode(Chimera::Node *_pNode) {

    switch(_pNode->getKind()){
        case Chimera::EntityKind::CAMERA:
            vCamera.push_back(_pNode);
            break;

        case Chimera::EntityKind::MESH:
            vMesh.push_back(_pNode);
            break;

        case Chimera::EntityKind::LIGHT:
            vLight.push_back(_pNode);
            break; 
    }
}
}
