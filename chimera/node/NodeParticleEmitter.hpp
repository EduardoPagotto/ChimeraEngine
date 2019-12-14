#ifndef __CHIMERA_NODE_PARTICLE_EMITTER__HPP
#define __CHIMERA_NODE_PARTICLE_EMITTER__HPP

#include "Node.hpp"
#include "Particle.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Timer.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/render/Material.hpp"
#include <vector>

// Verificar para implementar usando GLSL -
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
// Verificar para implementar usando GLSL - http://ogldev.atspace.co.uk/
// Verificar para implementar usando GLSL -
// http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

namespace Chimera {

#define MaxParticles 500

class NodeParticleEmitter : public Node {
  public:
    NodeParticleEmitter(Node* _parent, std::string _name, int _max);
    virtual ~NodeParticleEmitter();

    virtual void init() override;
    virtual void accept(class VisitorInterface* v) override;

    virtual void render(Shader* _pShader);

    glm::vec3 CameraPosition;

    void loadTexDiffuse(const std::string& _nome, const std::string& _arquivo);

    inline Transform* getTransform() const { return pTransform; }
    void setTransform(Transform* _pTransform) { pTransform = _pTransform; }

  private:
    Transform* pTransform;
    void SortParticles();
    int recycleParticleLife(const glm::vec3& _camPosition);
    int FindUnusedParticle();

    GLfloat* g_particule_position_size_data;
    GLubyte* g_particule_color_data;

    int LastUsedParticle;
    float lastTime;

    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLuint VertexArrayID;

    glm::vec3 sizeBox;
    Particle ParticlesContainer[MaxParticles];
    Timer timer;

    Material* material;
};
} // namespace Chimera
#endif
