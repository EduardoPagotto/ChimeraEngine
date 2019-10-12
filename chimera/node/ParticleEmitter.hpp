#ifndef __CHIMERA_PARTICLE_EMITTER__HPP
#define __CHIMERA_PARTICLE_EMITTER__HPP

#include "Draw.hpp"
#include "Particle.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Timer.hpp"
#include <vector>

// Verificar para implementar usando GLSL -
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
// Verificar para implementar usando GLSL - http://ogldev.atspace.co.uk/
// Verificar para implementar usando GLSL -
// http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

namespace Chimera {

#define MaxParticles 500

class ParticleEmitter : public Draw {
  public:
    ParticleEmitter(Node* _parent, std::string _name, int _max);
    virtual ~ParticleEmitter();

    // Inherited via Node
    virtual void accept(class NodeVisitor* v) override;

    // Inherited via Draw
    virtual void render(Shader* _pShader) override;
    virtual glm::vec3 getSizeBox() override;

    void setSizeBox(const glm::vec3& _size);

    void init();

    glm::vec3 CameraPosition;

    void loadTexDiffuse(const std::string& _nome, const std::string& _arquivo);

  private:
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
};
} // namespace Chimera
#endif