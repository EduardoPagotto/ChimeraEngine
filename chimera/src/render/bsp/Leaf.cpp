#include "chimera/render/bsp/Leaf.hpp"

namespace Chimera {

Leaf::Leaf() {}

Leaf::~Leaf() {
    while (index.empty() == false) {
        index.clear();
    }
}

void Leaf::addFace(uint32_t face, uint32_t _a, uint32_t _b, uint32_t _c) {
    index.push_back(_a);
    index.push_back(_b);
    index.push_back(_c);
    faces.push_back(face);
    // TODO: implementar acerto do AABB
}
} // namespace Chimera