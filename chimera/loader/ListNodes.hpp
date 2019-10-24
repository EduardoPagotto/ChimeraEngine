#ifndef __CHIMERA_LOADER_LIST_NODES__HPP
#define __CHIMERA_LOADER_LIST_NODES__HPP

#include <vector>

#include "chimera/core/Singleton.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/Solid.hpp"
//#include "chimera/node/Transform.hpp"
#include "chimera/node/Mesh.hpp"

namespace ChimeraLoaders {

class ListNodes {
    friend class Chimera::Singleton<ListNodes>;

  public:
    Chimera::Node* getByName(const Chimera::EntityKind& _kind, const std::string& _name) const;
    void addNode(Chimera::Node* _pNode);
    void reset() noexcept;

    inline void addSolid(const std::string& _name, Chimera::Solid* _pSolid) { mapSolids[_name] = _pSolid; }
    // inline void addTransform(const std::string& _name, Chimera::Transform* _pTrans) { mapTransform[_name] = _pTrans;
    // }
    inline void addMesh(const std::string& _name, Chimera::Mesh* _pMesh) { mapMesh[_name] = _pMesh; }

    std::map<std::string, Chimera::Mesh*> mapMesh;
    std::map<std::string, Chimera::Solid*> mapSolids;

  protected:
    ListNodes() noexcept;
    virtual ~ListNodes() { reset(); }
    ListNodes(const ListNodes&) = delete;

  private:
    // std::map<Chimera::EntityKind, std::vector<Chimera::Node*>> mapaKindNode;
    std::vector<Chimera::Node*> vMesh;
    std::vector<Chimera::Node*> vLight;
    std::vector<Chimera::Node*> vCamera;
    // std::vector<Chimera::Node*> vTransform;
    // std::vector<Chimera::Node*> vSolid;
    // std::map<std::string, Chimera::Solid*> mapSolids;
    // std::map<std::string, Chimera::Transform*> mapTransform;
    // std::map<std::string, Chimera::Mesh*> mapMesh;
};
} // namespace ChimeraLoaders
#endif