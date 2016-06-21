#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;

Node::Node ( EntityKind _type, std::string _id, std::string _name ) : Entity ( _type ) {

    name = _name;
    id = _id;
    parent = nullptr;
    listNode.push_back ( this );

}

Node::Node ( const Node &_node ) : Entity ( _node ) {

    name = _node.name;
    id = _node.id;
    parent = nullptr;

    listNode.push_back ( this );

}

Node::~Node() {

    parent = nullptr;
    listChild.clear();
    listNode.remove ( this );

}

void Node::clone ( Node **ppNode ) {

    for ( Node *pNode : listChild ) {

        Node *pNovo = nullptr;
        Node *pClone = *ppNode;

        pNode->clone ( &pNovo );

        pClone->addChild ( pNovo );

    }

}

void Node::addChild ( Node *child ) {

    listChild.push_back ( child );
    child->parent = this;

}

void Node::update ( DataMsg *dataMsg ) {

    for ( Node *node : listChild ) {

        if ( dataMsg->isDone() == true ) {
            break;
        }

        node->update ( dataMsg );

    }
}

Node *Node::findChildByKind ( EntityKind type, int index ) {

    int l_index = 0;
    for ( Node* node : listChild ) {

        if ( node->getKind() == type ) {
            if ( l_index == index ) {
                return node;
            }

            l_index++;
        }
    }
    return nullptr;
}

Node *Node::findNodeByName ( EntityKind type, std::string name ) {

    for ( Node *node : listNode ) {
        std::string l_name = node->getName();
        if ( ( node->getKind() == type ) && ( l_name.compare ( name ) == 0 ) ) {
            return node;
        }
    }

    return nullptr;
}

Node *Node::findNodeByName ( std::string name ) {

    for ( Node *node : listNode ) {
        std::string l_name = node->getName();
        if ( l_name.compare ( name ) == 0 ) {
            return node;
        }
    }

    return nullptr;
}

Node *Node::findNodeById ( EntityKind type, std::string id ) {

    for ( Node *node : listNode ) {
        std::string l_id = node->getId();
        if ( ( node->getKind() == type ) && ( l_id.compare ( id ) == 0 ) ) {
            return node;
        }
    }

    return nullptr;
}

Node *Node::findNodeById ( std::string id ) {

    for ( Node *node : listNode ) {
        std::string l_id = node->getId();
        if ( l_id.compare ( id ) == 0 ) {
            return node;
        }
    }
    return nullptr;
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
