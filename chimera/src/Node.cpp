#include "Node.h"

namespace Chimera {

Node::Node(EntityType type, std::string name) : Entity(type) {
    parent = nullptr;
    setName(name);
}

Node::~Node() {
    parent = nullptr;
    lChild.clear();
}

void Node::addChild(Node *child) {

    lChild.push_back(child);
    child->parent = this;

}

void Node::update ( DataMsg *dataMsg ){

    for (Node *node : lChild) {
        
        if (dataMsg->isDone() == true)
            break;
        
        node->update(dataMsg);
        
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
