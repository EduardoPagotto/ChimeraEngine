#ifndef __CHIMERA_LOADER_VISUAL_SCENE__HPP
#define __CHIMERA_LOADER_VISUAL_SCENE__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/node/NodeGroup.hpp"

namespace Chimera {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, NodeGroup* _pGroup);
    virtual ~VisualScene();
    void target();

  private:
    ListNodes* pListNodes;
    NodeGroup* pGroup;
};
} // namespace Chimera

#endif