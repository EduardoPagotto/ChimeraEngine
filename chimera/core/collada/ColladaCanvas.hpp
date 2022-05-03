#pragma once
#include "chimera/core/collada/ColladaDom.hpp"
#include "chimera/core/device/CanvasGL.hpp"

namespace Chimera {
CanvasGL* colladaCanvasGL(ColladaDom& dom);
}
