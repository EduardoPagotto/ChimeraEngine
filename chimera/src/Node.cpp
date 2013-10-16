#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;
    
Node::Node(EntityKind _type) : Entity(_type) {
    
    name = "name-Node";
    id = "id-Node";
    parent = nullptr;
    listNode.push_back(this);
    
}

Node::Node (const Node &_node) : Entity(_node) {
    
    name = _node.name;
    id = _node.id;
    parent = nullptr;
    
    //FIXME preciso copiar mesmo???
    //if (_node.parent != nullptr) {
    //    parent = _node.parent;
    //    _node.parent->addChild(this);
    //}
        
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

Node *Node::findObjByName ( EntityKind type,std::string name ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_name = node->getName();
        if ( ( node->getKind() ==type ) && ( l_name.compare ( name ) == 0 ) )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjByName ( std::string name ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_name = node->getName();
        if ( l_name.compare ( name ) == 0 )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjById ( EntityKind type,std::string id ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_id = node->getId();
        if ( ( node->getKind() == type ) && ( l_id. compare ( id ) == 0 ) )
            return node;
        
    }
    
    return nullptr;
}

Node *Node::findObjById ( std::string id ) {
    
    for ( Node *node : listNode ) {
        
        std::string l_id = node->getId();
        if ( l_id.compare ( id ) == 0 )
            return node;
        
    }
    
    return nullptr;
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
