#include "chimera/node/VisitParser.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/VisitorInterface.hpp"

namespace Chimera {

void visitParserTree(class Node* u, class VisitorInterface* pVisit) {

    u->setColor(1);
    u->accept(pVisit);

    std::vector<Node*>* children = u->getChilds();
    if ((children != nullptr) && (children->size() > 0)) {

        for (size_t i = 0; i < children->size(); i++) {

            Node* child = children->at(i);
            if (child->getColor() == 0)
                visitParserTree(child, pVisit);
        }
    }
    u->setColor(0);
}
} // namespace Chimera
