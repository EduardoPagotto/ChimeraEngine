#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, Scene* scene) : Library(nullptr, "!" + _file), scene(scene) {}
    virtual ~VisualScene(){};
    void target();

  private:
    Scene* scene;
};
} // namespace Chimera
