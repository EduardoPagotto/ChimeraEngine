#ifndef __CHIMERA_LOADER_LIST_NODES__HPP
#define __CHIMERA_LOADER_LIST_NODES__HPP

#include <vector>

#include "Node.hpp"
#include "Singleton.hpp"

namespace ChimeraLoaders {

class ListNodes {
    friend class Chimera::Singleton<ListNodes>;

  public:
    Chimera::Node* getByName(const Chimera::EntityKind& _kind,
                             const std::string& _name) const;
    void addNode(Chimera::Node* _pNode);
    void reset() noexcept;

  protected:
    ListNodes() noexcept;
    virtual ~ListNodes() { reset(); }
    ListNodes(const ListNodes&) = delete;

  private:
    // std::map<Chimera::EntityKind, std::vector<Chimera::Node*>> mapaKindNode;
    std::vector<Chimera::Node*> vMesh;
    std::vector<Chimera::Node*> vLight;
    std::vector<Chimera::Node*> vCamera;
    std::vector<Chimera::Node*> vTransform;
    std::vector<Chimera::Node*> vSolid;
};
} // namespace ChimeraLoaders
#endif