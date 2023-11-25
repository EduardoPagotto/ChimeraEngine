#include "chimera/core/Registry.hpp"

namespace Chimera {

Entity Registry::createEntity(const std::string& tag, const std::string& id) {
    Entity entity = {eRegistry.create(), this};
    auto& ec = entity.addComponent<TagComponent>();
    ec.tag = tag.empty() ? "Entity" : tag;
    ec.id = id;
    ec.serial = Registry::masterSerial;
    Registry::masterSerial++;
    return entity;
}

void Registry::destroyEntity(Entity entity) { eRegistry.destroy(entity); }
} // namespace Chimera