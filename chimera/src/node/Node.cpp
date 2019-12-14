#include "chimera/node/Node.hpp"

namespace Chimera {

Node::Node(Node* _parent, const Kind& _type, const std::string& _name)
    : parent(_parent), type(_type), name(_name), serial(++serialMaster) {
    if (parent != nullptr)
        parent->vChild.push_back(this);

    setColor(0);
}

Node::~Node() {
    parent = nullptr;
    vChild.clear();
}

void Node::initializeChilds() {
    this->init();
    for (Node* node : vChild) {
        node->initializeChilds();
    }
}

std::vector<Node*>* Node::getChilds() {
    if (!vChild.empty())
        return &(this->vChild);

    return nullptr;
}

void Node::addChild(Node* _child) {
    if (_child != nullptr) {
        _child->setParent(this);
        vChild.push_back(_child);
    }
}

void Node::setParent(Node* _node) {
    if (parent != nullptr)
        parent->removeChild(this);

    parent = _node;
}

void Node::removeChild(Node* _child) {
    if (_child != nullptr && !vChild.empty()) {
        for (size_t i = 0; i < vChild.size(); ++i) {
            if (vChild[i] == _child) {
                vChild.erase(vChild.begin() + i);
                break;
            }
        }
    }
};

const size_t Node::countChilds(const bool& _recursiveCount) const {
    if (!_recursiveCount) {
        return (vChild.size());
    } else {
        size_t Retval = vChild.size();
        for (size_t i = 0; i < vChild.size(); ++i) {
            Retval += vChild[i]->countChilds(true);
        }
        return (Retval);
    }
};

Node* Node::findChild(const std::string& _searchName, const bool& _findInChild) {
    if (!vChild.empty()) {
        for (size_t i = 0; i < vChild.size(); ++i) {
            if (_searchName.compare(vChild[i]->getName()) == 0)
                return vChild[i];
        }

        if (_findInChild == true) {
            for (size_t i = 0; i < vChild.size(); ++i) {

                Node* retVal = vChild[i]->findChild(_searchName, _findInChild);

                if (retVal != nullptr)
                    return retVal;
            }
        }
    }
    return nullptr;
};

Node* Node::findChild(const Kind& _type, const int& _index, const bool& _findInChild) {
    int l_index = 0;
    for (Node* node : vChild) {
        if (node->getKind() == _type) {
            if (l_index == _index)
                return node;

            l_index++;
        }
    }

    if (_findInChild == true) {
        for (Node* node : vChild) {
            Node* result = node->findChild(_type, _index, _findInChild);
            if (result != nullptr)
                return result;
        }
    }
    return nullptr;
}
unsigned Node::serialMaster = 0;
} // namespace Chimera
