#include "chimera/collada/ColladaParticle.hpp"
#include "chimera/core/gl/ParticleEmitter.hpp"

namespace ce {
    void ColladaParticle::create(const std::string& id, const std::string& name, Entity& entity,
                                 pugi::xml_node nParticle) {

        glm::vec dir = glm::vec3(0, 0, 10);
        float spread = 1.5f;
        const pugi::xml_node& nEmiter = nParticle.child("emmiter_font");
        setChildParam(nEmiter, "maindir", dir);
        setChildParam(nEmiter, "spread", spread);

        EmitterComponent& ec = entity.addComponent<EmitterComponent>();
        ec.tag.id = id;
        ec.tag.name = name;
        ec.emitter = new EmitterFont(dir, spread); // EF to R

        ParticleContainer* pc = new ParticleContainer();
        const pugi::xml_node& nContainer = nParticle.child("container");
        setChildParam(nContainer, "life", pc->life);
        setChildParam(nContainer, "max", pc->max);
        setChildParam(nContainer, "respaw", pc->respaw);
        ec.emitter->pushParticleContainer(pc);
    }
} // namespace ce
