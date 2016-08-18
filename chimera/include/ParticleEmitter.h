#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include<vector>

#include "Particle.h"
# include "Draw.h"
# include "Texture.h"

#include "Shader.h"

#include "Timer.h"

// Verificar para implementar usando GLSL - http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
// Verificar para implementar usando GLSL - http://ogldev.atspace.co.uk/
// Verificar para implementar usando GLSL - http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

namespace Chimera {
   
#define MaxParticles 100000

class ParticleEmitter : public Draw
{
public:
	ParticleEmitter(Node *_parent, std::string _name, int _max);
	virtual ~ParticleEmitter();

	virtual void accept(class NodeVisitor* v);
	void SortParticles();
	virtual void renderExecute(bool _texture) override;

	virtual glm::vec3 getSizeBox() override;
	
	void setSizeBox(const glm::vec3 &_size);

	void init();

	void setShader(const Shader &_shader) {
		shader = _shader;
	}

private:
	int FindUnusedParticle();


	glm::vec3 sizeBox;

	Shader shader;

	GLfloat* g_particule_position_size_data;// = new GLfloat[MaxParticles * 4];
	GLubyte* g_particule_color_data;// = new GLubyte[MaxParticles * 4];

	//int MaxParticles;
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle;

	float lastTime;

	// Vertex shader
	GLuint CameraRight_worldspace_ID;// = glGetUniformLocation(shader.getIdProgram, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID;// = glGetUniformLocation(shader.getIdProgram, "CameraUp_worldspace");
	GLuint ViewProjMatrixID;// = glGetUniformLocation(shader.getIdProgram, "VP");

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	GLuint Texture;
	GLuint TextureID;

	Timer timer;
};




//class ParticleEmitter : public Draw
//{
//public:
//    ParticleEmitter (Node *_parent, std::string _name );
//    virtual ~ParticleEmitter();
//	virtual void init();
//	virtual void accept(class NodeVisitor* v);
//    virtual void renderExecute(bool _texture);
//    virtual glm::vec3 getSizeBox();
//    
//    void setGL();
//    void loadImage ( const char *_file );
//    void SortParticles ( const glm::vec3 &posCamera );
//    void setSizeBox(const glm::vec3 &_size);
//
//	inline void setPosSource(const glm::vec3 &_val) {
//		posSource = _val;
//	}
//
//	inline glm::vec3 getPosSource() const {
//		return posSource;
//	}
//
//	inline void setMaxSeed(const int &_max) {
//		maxSeed = _max;
//	}
//
//	inline int getMaxSeed() const {
//		return maxSeed;
//	}
//
//	inline void setPosition(const glm::vec3 &_pos) {
//		position = _pos;
//	}
//
//private:
//	glm::vec3 position;
//	glm::vec3 posSource;
//    glm::vec3 sizeBox;
//    
//	int maxSeed;
//    
//    std::vector<Particle*> particles;
//    std::vector<Color> coresPart;
//};
}
#endif
