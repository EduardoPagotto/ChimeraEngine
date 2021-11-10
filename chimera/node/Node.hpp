#pragma once
#include <cstdint>
#include <list>
#include <string>
#include <vector>

namespace Chimera {

enum class Kind : int8_t { ROOT, GROUP, CAMERA, LIGHT, SKYBOX, HUD, MESH, PARTICLE_SYSTEM };

class Node {
  public:
    Node() : parent(nullptr), type(Kind::ROOT), name("root"), serial(++serialMaster), color(0) {}
    Node(Node* _parent, const Kind& _type, const std::string& _name);
    virtual ~Node();

    virtual void accept(class VisitorInterface* v){};
    virtual void init(){};

    void initializeChilds();

    void addChild(Node* _child);
    void removeChild(Node* _child);

    void setParent(Node* _node);
    inline Node* getParent() { return parent; }

    inline void setColor(const int& _col) { color = _col; }
    inline int getColor() const { return color; }

    const size_t countChilds(const bool& _recursiveCount) const;

    Node* findChild(const std::string& _searchName, const bool& _findInChild);
    Node* findChild(const Kind& _type, const int& _index, const bool& _findInChild);

    inline Kind getKind() const { return type; }

    inline const std::string& getName() const { return name; }
    inline void setName(const std::string& name) { this->name = name; }

    static unsigned getNextSerialMaster() { return Node::serialMaster; }

    std::vector<Node*>* getChilds();

  private:
    int color;
    unsigned serial;
    static unsigned serialMaster;
    Kind type;

    Node* parent;
    std::vector<Node*> vChild;
    std::string name;
};
} // namespace Chimera
