#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, Registry* reg) : Library(nullptr, "!" + _file), reg(reg) {}
    virtual ~VisualScene(){};
    void target();

  private:
    Registry* reg;
};
} // namespace Chimera
