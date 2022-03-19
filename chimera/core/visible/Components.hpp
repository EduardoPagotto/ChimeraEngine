#pragma once
#include "ITrans.hpp"

namespace Chimera {

struct ComponentTrans {
    ITrans* trans = nullptr;
    bool solid = false;
    ComponentTrans() = default;
};
} // namespace Chimera