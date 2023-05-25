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

Entity Registry::findEntityTag(const std::string& tagName) {
    auto view = eRegistry.view<TagComponent>();
    for (auto ent : view) {
        TagComponent& ee = eRegistry.get<TagComponent>(ent);
        if (ee.tag == tagName)
            return {ent, this};
    }
    return Entity();
}

Entity Registry::findEntityId(const std::string& id) {
    auto view = eRegistry.view<TagComponent>();
    for (auto ent : view) {
        TagComponent& ee = eRegistry.get<TagComponent>(ent);
        if (ee.id == id)
            return {ent, this};
    }
    return Entity();
}

Entity Registry::findEntitySerial(const uint32_t& serial) {
    auto view = eRegistry.view<TagComponent>();
    for (auto ent : view) {
        TagComponent& ee = eRegistry.get<TagComponent>(ent);
        if (ee.serial == serial)
            return {ent, this};
    }
    return Entity();
}

} // namespace Chimera