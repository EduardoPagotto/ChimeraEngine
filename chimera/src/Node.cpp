#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;

Node::Node ( Node *_parent, EntityKind _type, std::string _name )
    : parent ( _parent ), Entity ( _type, _name ) {

    if ( parent != nullptr ) {
        parent->vChild.push_back ( this );
    }

    listNode.push_back ( this );

    setColor ( 0 );
}

Node::~Node() {
    parent = nullptr;
    vChild.clear();
}

std::vector<Node*>* Node::getChilds() {
    if ( !vChild.empty() ) {
        return & ( this->vChild );
    }

    return nullptr;
}

void Node::addChild ( Node *_child ) {

    if ( _child != nullptr ) {
        if ( _child->getParent() == nullptr ) {
            _child->setParent ( this );
        }

        vChild.push_back ( _child );
    }
}

void Node::setParent ( Node *_node ) {

    if ( parent != nullptr ) {
        parent->removeChild ( this );
    }

    parent = _node;
}

void Node::removeChild ( Node* _child ) {
    if ( _child != nullptr  && !vChild.empty() ) {
        for ( size_t i = 0; i < vChild.size(); ++i ) {
            if ( vChild[i] == _child ) {
                vChild.erase ( vChild.begin() + i );
                break;
            }
        }
    }
};

const size_t Node::countChilds ( const bool &_recursiveCount ) const {
    if ( !_recursiveCount ) {

        return ( vChild.size() );

    } else {

        size_t Retval = vChild.size();
        for ( size_t i = 0; i < vChild.size(); ++i ) {
            Retval += vChild[i]->countChilds ( true );
        }

        return ( Retval );
    }
};

Node* Node::findChild ( const std::string &_searchName ) {
    Node* Retval = NULL;
    if ( !vChild.empty() ) {
        for ( size_t i = 0; i < vChild.size(); ++i ) {
            if ( _searchName.compare ( vChild[i]->getName() ) == 0 ) {
                Retval = vChild[i];
                break; // break the for loop
            }
        }
    }
    return ( Retval );
};

Node *Node::findChild ( const EntityKind &_type, const int &_index ) {

    int l_index = 0;
    for ( Node* node : vChild ) {

        if ( node->getKind() == _type ) {
            if ( l_index == _index ) {
                return node;
            }

            l_index++;
        }
    }
    return nullptr;
}

Node *Node::findNodeBySeq ( const EntityKind &_type, const int &_index ) {

    int l_index = 0;
    for ( Node* node : listNode ) {

        if ( node->getKind() == _type ) {
            if ( l_index == _index ) {
                return node;
            }

            l_index++;
        }
    }
    return nullptr;
}

Node *Node::findNodeBySeq ( const EntityKind &_type, const std::string &_name ) {

    for ( Node *node : listNode ) {
        std::string l_name = node->getName();
        if ( ( node->getKind() == _type ) && ( l_name.compare ( _name ) == 0 ) ) {
            return node;
        }
    }

    return nullptr;
}

unsigned Entity::serialMaster = 0;

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
