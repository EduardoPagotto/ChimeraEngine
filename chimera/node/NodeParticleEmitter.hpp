#ifndef __CHIMERA_NODE_PARTICLE_EMITTER__HPP
#define __CHIMERA_NODE_PARTICLE_EMITTER__HPP

#include "Node.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/io/Timer.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Particle.hpp"
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

    glm::vec4* vPosSize;
    GLubyte* vColor;

    int LastUsedParticle;
    float lastTime;

    GLuint vboVertex;
    GLuint vboPosition;
    GLuint vboColor;
    GLuint vao;

    glm::vec3 sizeBox;
    Particle ParticlesContainer[MaxParticles];
    Core::Timer timer;

    Material* material;
};
} // namespace Chimera
#endif
