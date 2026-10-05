#include "chimera_collada/ColladaParticle.hpp"
#include "chimera_ecs/EmitterComponent.hpp"

namespace ce {
    void ColladaParticle::create(const std::string& id, const std::string& name, Entity& entity,
                                 pugi::xml_node n_particle) {

        glm::vec dir = glm::vec3(0, 0, 10);
        float spread = 1.5f;
        const pugi::xml_node& n_emiter = n_particle.child("emmiter_font");
        setChildParam(n_emiter, "maindir", dir);
        setChildParam(n_emiter, "spread", spread);

        EmitterComponent& ec = entity.add_component<EmitterComponent>(registry.get());
        ec.tag.id = id;
        ec.tag.name = name;
        ec.emitter = new EmitterFont(dir, spread); // EF to R

        std::shared_ptr<ParticleContainer> pc = std::make_shared<ParticleContainer>();
        const pugi::xml_node& n_container = n_particle.child("container");
        setChildParam(n_container, "life", pc->life);
        setChildParam(n_container, "max", pc->max);
        setChildParam(n_container, "respaw", pc->respaw);
        ec.emitter->push_particle_container(pc);
    }
} // namespace ce
