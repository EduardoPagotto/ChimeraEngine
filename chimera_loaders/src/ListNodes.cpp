#include "ListNodes.h"

namespace ChimeraLoaders {

ListNodes::ListNodes() noexcept {
}

void ListNodes::reset() noexcept {
    vCamera.clear();
    vMesh.clear();
    vLight.clear();
    vTransform.clear();
}

Chimera::Node* ListNodes::getByName(const Chimera::EntityKind &_kind, const std::string &_name) const {

    switch(_kind){

        case Chimera::EntityKind::CAMERA:
            for ( Chimera::Node *node : vCamera )
                if (node->getName().compare(_name) == 0)
                    return node;
            break;

        case Chimera::EntityKind::MESH:
            for ( Chimera::Node *node : vMesh )
                if (node->getName().compare(_name) == 0)
                    return node;
            break;

        case Chimera::EntityKind::LIGHT:
            for ( Chimera::Node *node : vLight )
                if (node->getName().compare(_name) == 0)
                    return node;
            break;

        case Chimera::EntityKind::TRANSFORM:
            for ( Chimera::Node *node : vTransform )
                if (node->getName().compare(_name) == 0)
                    return node;
            break;

        case Chimera::EntityKind::SOLID:
            for ( Chimera::Node *node : vSolid )
                if (node->getName().compare(_name) == 0)
                    return node;
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

        case Chimera::EntityKind::TRANSFORM:
            vTransform.push_back(_pNode);
            break;

        case Chimera::EntityKind::SOLID:
            vSolid.push_back(_pNode);
            break;

    }
}
}
