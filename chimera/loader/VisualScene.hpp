#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, Scene* scene);
    virtual ~VisualScene();
    void target();

  private:
    ListNodes* pListNodes;
    Scene* scene;
};
} // namespace Chimera
