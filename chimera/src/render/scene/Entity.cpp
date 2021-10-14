#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

Entity::Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene) {}

} // namespace Chimera