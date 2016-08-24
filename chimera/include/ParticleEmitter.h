#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include<vector>
#include "Particle.h"
#include "Draw.h"
#include "Texture.h"
#include "Shader.h"
#include "Timer.h"

// Verificar para implementar usando GLSL - http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
// Verificar para implementar usando GLSL - http://ogldev.atspace.co.uk/
// Verificar para implementar usando GLSL - http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

namespace Chimera {

#define MaxParticles 500

class ParticleEmitter : public Draw
{
public:
	ParticleEmitter(Node *_parent, std::string _name, int _max);
	virtual ~ParticleEmitter();

	// Inherited via Node
	virtual void accept(class NodeVisitor* v) override;
	
	// Inherited via Draw
	virtual void render() override;
	virtual glm::vec3 getSizeBox() override;

	void setSizeBox(const glm::vec3 &_size);

	void init();

	glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;

private:
	void SortParticles();
	int recycleParticleLife(const glm::vec3 &_camPosition);

	int FindUnusedParticle();

	glm::vec3 sizeBox;

	Shader* shader;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle;

	float lastTime;
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
    GLuint VertexArrayID;

	Timer timer;
};
}
#endif
