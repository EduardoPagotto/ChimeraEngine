#include "ParticleEmitter.h"
#include "ExceptionChimera.h"
#include "Coord.h"
#include "Camera.h"
#include "NodeVisitor.h"
#include <algorithm>

namespace Chimera {
ParticleEmitter::ParticleEmitter(Node * _parent, std::string _name, int _max) : Draw(_parent, EntityKind::PARTICLE_SYSTEM, _name){
	//MaxParticles = _max;
	LastUsedParticle = 0;
}

ParticleEmitter::~ParticleEmitter(){
}

void ParticleEmitter::init() {

	pState->init();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
    
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);

	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);

	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	timer.start();
}

void ParticleEmitter::setSizeBox ( const glm::vec3& _size ) {
	sizeBox = _size;
}
	
glm::vec3 ParticleEmitter::getSizeBox() {
	return sizeBox;
}
	
void ParticleEmitter::accept(NodeVisitor * v) {
	v->visit(this);
}

void ParticleEmitter::SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


int ParticleEmitter::recycleParticleLife(const glm::vec3 &_camPosition) {

	//Uint32 delta = 1;//timer.delta() / 50;
	double delta = 1.0f / timer.delta();

	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);

	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i < newparticles; i++) {

		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].reset();

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 10.0f);

		ParticlesContainer[particleIndex].create(maindir, spread);
	}

	// Simulate all particles
	int ParticlesCount = 0;

	for (int i = 0; i<MaxParticles; i++) {

		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.isDead() == false) {

			p.decrease(delta, ParticlesCount, g_particule_position_size_data, g_particule_color_data, _camPosition);
			ParticlesCount++;
		}
	}

	SortParticles();

	printf("%d \n",ParticlesCount);

	return ParticlesCount;
}

void ParticleEmitter::renderExecute(bool _texture)
{
	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	// There should be a getCameraPosition() function in common/controls.cpp, 
	// but this works too.
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	int ParticlesCount = recycleParticleLife(CameraPosition);

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

    glBindVertexArray(VertexArrayID);//coloquei aqui porque acho que tenho que ligar antes 
    
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	shader.link();//glUseProgram(programID);

	// Vertex shader
    GLuint CameraRight_worldspace_ID = glGetUniformLocation(shader.getIdProgram(), "CameraRight_worldspace");
    GLuint CameraUp_worldspace_ID = glGetUniformLocation(shader.getIdProgram(), "CameraUp_worldspace");
    GLuint ViewProjMatrixID = glGetUniformLocation(shader.getIdProgram(), "VP");

    // fragment shader
    GLuint TextureID = glGetUniformLocation(shader.getIdProgram(), "myTextureSampler");

    GLuint Texture = pState->getMaterial()->getTexDiffuse()->getTextureId(0);//loadDDS("particle.DDS");
    
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
    
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// Same as the billboards tutorial
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

								 // Draw the particules !
								 // This draws many times a small triangle_strip (which looks like a quad).
								 // This is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster.
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

int ParticleEmitter::FindUnusedParticle() {

	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if ( ParticlesContainer[i].isDead() == true ) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].isDead() == true) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}
}