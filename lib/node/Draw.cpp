#include "Draw.hpp"

namespace Chimera {

Draw::Draw(Node* _parent, EntityKind _kind, std::string _name)
    : Node(_parent, _kind, _name) {
    material = nullptr;
}

Draw::Draw(const Draw& _draw) : Node(_draw) { material = _draw.material; }

Draw::~Draw() { material = nullptr; }
} // namespace Chimera
