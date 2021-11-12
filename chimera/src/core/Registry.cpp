// #include "chimera/core/Registry.hpp"

// namespace Chimera {
// entt::registry Registry::eRegistry;

// Entity Registry::CreateEntity(const std::string& name) {

//     // toda entidade tem um transform
//     Entity entity = {eRegistry.create()};
//     // entity.addComponent<Transform>();
//     auto& tag = entity.addComponent<TagComponent>();
//     tag.tag = name.empty() ? "Entity" : name;

//     return entity;
// }

// void Registry::DestroyEntity(Entity entity) { eRegistry.destroy(entity); }

// } // namespace Chimera