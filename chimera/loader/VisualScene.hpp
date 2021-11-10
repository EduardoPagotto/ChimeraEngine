#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, NodeGroup* _pGroup, Scene* scene);
    virtual ~VisualScene();
    void target();

  private:
    ListNodes* pListNodes;
    NodeGroup* pGroup;

    Scene* scene;
};
} // namespace Chimera
