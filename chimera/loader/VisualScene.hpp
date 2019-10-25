#ifndef __CHIMERA_LOADER_VISUAL_SCENE__HPP
#define __CHIMERA_LOADER_VISUAL_SCENE__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/node/Group.hpp"

namespace ChimeraLoaders {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, Chimera::Group* _pGroup);
    virtual ~VisualScene();
    void target();

  private:
    ListNodes* pListNodes;
    Chimera::Group* pGroup;
};
} // namespace ChimeraLoaders

#endif