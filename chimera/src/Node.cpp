#include "Node.h"

namespace Chimera {

std::list<Node*> Node::listNode;
    
Node::Node() : Entity(EntityKind::NODE) {

    name = "name-Node";
    id = "id-Node";
    parent = nullptr;
    listNode.push_back(this);
    
    logger = log4cxx::Logger::getLogger ( "Node" );
    
    std::string l_msg = Node::getNameKindNode(getKind())+ " id:" + getId() + ", serial:"+ std::to_string( getSerial() )+ " Criado";
    LOG4CXX_INFO ( logger , l_msg );
    
}

Node::Node(EntityKind _type) : Entity(_type) {
    
    name = "name-Node";
    id = "id-Node";
    parent = nullptr;
    listNode.push_back(this);
    
    logger = log4cxx::Logger::getLogger ( "Node" );
    
    std::string l_msg = Node::getNameKindNode(getKind())+ " id:" + getId() + ", serial:"+ std::to_string( getSerial() )+ " Criado";
    LOG4CXX_INFO ( logger , l_msg );
    
}

Node::Node (const Node &_node) : Entity(_node) {
    
    name = _node.name;
    id = _node.id;
    parent = nullptr;
            
    listNode.push_back(this);
    
    logger = log4cxx::Logger::getLogger ( "Node" );
    
    std::string l_msg = Node::getNameKindNode(getKind())+ " id:" + getId() + ", serial:"+ std::to_string( getSerial() )+ " Criado, Origem:" + _node.getId();
    LOG4CXX_INFO ( logger , l_msg );
}

Node::~Node() {
    
    parent = nullptr;
    listChild.clear();
    listNode.remove ( this );
    
    std::string l_msg = Node::getNameKindNode(getKind())+ " id:" + getId() + ", serial:"+ std::to_string( getSerial() )+ " Destruido";
    LOG4CXX_INFO ( logger , l_msg );
}

void Node::addChild(Node *child) {

    listChild.push_back(child);
    child->parent = this;
    
    std::string l_msg = Node::getNameKindNode(child->getKind())+ 
                        " id:" + child->getId() + 
                        ", serial:"+ std::to_string( child->getSerial() )+ 
                        " anexado a " + Node::getNameKindNode(getKind()) +
                        " id:" + getId() + 
                        ", serial:"+ std::to_string( getSerial() );                        
       
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

std::string Node::getNameKindNode(EntityKind _kind) {
    
    switch (_kind) {
        case EntityKind::NODE:
            return std::string("NODE");
            break;
        case EntityKind::CAMERA:
            return std::string("CAMERA");
            break;
        case EntityKind::LIGHT:
            return std::string("LIGHT");
            break;
        case EntityKind::OBJECT:
            return std::string("OBJCT");
            break;
    }
    
    return std::string("Desconhecido");
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
