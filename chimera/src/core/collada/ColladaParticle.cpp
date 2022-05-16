#include "chimera/core/collada/ColladaParticle.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"

namespace Chimera {

ColladaParticle::~ColladaParticle() {}

void ColladaParticle::create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nParticle) {

    // loadMaterial(entity, "#vazio", nParticle);
    glm::vec3 dir = textToVec3(nParticle.child("emmiter_font").child("maindir").text().as_string());
    float spread = nParticle.child("emmiter_font").child("spread").text().as_float();

    EmitterComponent& ec = entity.addComponent<EmitterComponent>();
    ec.tag.id = id;
    ec.tag.tag = name;
    ec.emitter = new EmitterFont(dir, spread); // EF to R

    ParticleContainer* pc = new ParticleContainer();
    pc->life = nParticle.child("container").child("life").text().as_float();
    pc->max = nParticle.child("container").child("max").text().as_int();
    pc->respaw = nParticle.child("container").child("respaw").text().as_bool();
    ec.emitter->pushParticleContainer(pc);
}
} // namespace Chimera