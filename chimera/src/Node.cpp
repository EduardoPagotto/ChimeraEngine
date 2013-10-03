#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;
    
Node::Node(EntityType type, std::string id, std::string name) : Entity(type), id(id), name(name) {
    
    parent = nullptr;
    listNode.push_back(this);
    
}

Node::Node ( const Node &node ) : Entity(node), id(node.id), name(node.name) {
    
    parent = nullptr;
    listNode.push_back(this);
    
}

Node::~Node() {
    
    parent = nullptr;
    listChild.clear();
    listNode.remove ( this );
    
}

void Node::addChild(Node *child) {

    listChild.push_back(child);
    child->parent = this;

}

void Node::update ( DataMsg *dataMsg ){

    for (Node *node : listChild) {
        
        if (dataMsg->isDone() == true)
            break;
        
        node->update(dataMsg);
        
    }
}

Node *Node::findObjByName ( EntityType type,std::string name ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_name = node->getName();
        if ( ( node->getType() ==type ) && ( l_name.compare ( name ) == true ) )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjByName ( std::string name ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_name = node->getName();
        if ( l_name.compare ( name ) == true )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjById ( EntityType type,std::string id ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_id = node->getId();
        if ( ( node->getType() ==type ) && ( l_id.compare ( id ) == true ) )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjById ( std::string id ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_id = node->getId();
        if ( l_id.compare ( id ) == true )
            return node;
        
    }
    
    return nullptr;
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
