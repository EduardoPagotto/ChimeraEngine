#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;
    
Node::Node(EntityKind _type, std::string _id, std::string _name) : Entity(_type) {
    
    name = _name;
    id = _id;
    parent = nullptr;
    listNode.push_back(this);
    
    logger = log4cxx::Logger::getLogger ( "Node" );
    
    std::string l_msg = "NEW "+ Entity::getNameKindNode(getKind())+ ",Id:" + getId() + ",Name:" + getName() + ",Serial:"+ std::to_string( getSerial() );
    LOG4CXX_INFO ( logger , l_msg );
    
}

Node::Node (const Node &_node) : Entity(_node) {
    
    name = _node.name;
    id = _node.id;
    parent = nullptr;
            
    listNode.push_back(this);
    
    logger = log4cxx::Logger::getLogger ( "Node" );
    
    std::string l_msg = "COPY " + Entity::getNameKindNode(getKind())+ ",Id:" + getId() + ",Name:" + getName() + ",Serial:"+ std::to_string( getSerial() ) + ",Source:" + std::to_string( _node.getSerial()) ;
    LOG4CXX_INFO ( logger , l_msg );
}

Node::~Node() {
    
    parent = nullptr;
    listChild.clear();
    listNode.remove ( this );
    
    std::string l_msg = "DELETE " + Entity::getNameKindNode(getKind())+ ",Id:" + getId() + ",Serial:"+ std::to_string( getSerial() );
    LOG4CXX_INFO ( logger , l_msg );
}

void Node::addChild(Node *child) {

    listChild.push_back(child);
    child->parent = this;
    
    std::string l_msg = "CHILD " + Entity::getNameKindNode(child->getKind())+ 
                        ",serial:"+ std::to_string( child->getSerial() )+ 
                        " IN " + Entity::getNameKindNode(getKind()) +
                        ",serial:"+ std::to_string( getSerial() );                        
       
    LOG4CXX_INFO ( logger , l_msg );

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
