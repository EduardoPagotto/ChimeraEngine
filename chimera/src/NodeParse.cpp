#include "NodeParse.h"
#include "Node.h"
#include "NodeVisitor.h"

namespace Chimera {

void NodeParse::tree(class Node* u, class NodeVisitor* pVisit) {

    u->setColor(1);
    u->accept(pVisit);

    std::vector<Node*>* children = u->getChilds();
    if ((children != nullptr) && (children->size() > 0)) {

        for (size_t i = 0; i < children->size(); i++) {

            Node* child = children->at(i);
            if (child->getColor() == 0)
                tree(child, pVisit);
        }
    }
    u->setColor(0);
}

} // namespace Chimera
