#pragma once
#include "ITrans.hpp"
#include "Light.hpp"

namespace Chimera {

struct ComponentTrans {
    ITrans* trans = nullptr;
    bool solid = false;
    ComponentTrans() = default;
};

struct ComponentLight {
    Light* light = nullptr;
    bool global = true;
    ComponentLight() = default;
};
} // namespace Chimera