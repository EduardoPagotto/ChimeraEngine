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

	// Create and compile our GLSL program from the shaders
	//GLuint programID = LoadShaders("Particle.vertexshader", "Particle.fragmentshader");

	// Vertex shader
	//CameraRight_worldspace_ID = glGetUniformLocation(shader.getIdProgram(), "CameraRight_worldspace");
	//CameraUp_worldspace_ID = glGetUniformLocation(shader.getIdProgram(), "CameraUp_worldspace");
	//ViewProjMatrixID = glGetUniformLocation(shader.getIdProgram(), "VP");

	// fragment shader
	//TextureID = glGetUniformLocation(shader.getIdProgram(), "myTextureSampler");

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	//no contrutor do particle
	//for (int i = 0; i<MaxParticles; i++) {
	//	ParticlesContainer[i].life = -1.0f;
	//	ParticlesContainer[i].cameradistance = -1.0f;
	//}

	

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	//GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	//GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	//GLuint particles_color_buffer;
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

void ParticleEmitter::renderExecute(bool _texture)
{
	//double currentTime = glfwGetTime();
	//double delta = currentTime - lastTime;
	//lastTime = currentTime;

	//computeMatricesFromInputs();
	//glm::mat4 ProjectionMatrix = getProjectionMatrix();
	//glm::mat4 ViewMatrix = getViewMatrix();
	//double currentTime = 0;//glfwGetTime();
	Uint32 delta = timer.delta();//currentTime - lastTime;
	//lastTime = currentTime;

	//computeMatricesFromInputs();
	//glm::mat4 ProjectionMatrix;// = getProjectionMatrix();
	//glm::mat4 ViewMatrix;// = getViewMatrix();

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	// There should be a getCameraPosition() function in common/controls.cpp, 
	// but this works too.
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);

	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i <newparticles; i++) {

		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].reset();  //ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
													//ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

	//---
		//// Very bad way to generate a random direction; 
		//// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		//// combined with some user-controlled parameters (main direction, spread, etc)
		//glm::vec3 randomdir = glm::vec3(
		//	(rand() % 2000 - 1000.0f) / 1000.0f,
		//	(rand() % 2000 - 1000.0f) / 1000.0f,
		//	(rand() % 2000 - 1000.0f) / 1000.0f
		//);

		//ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;


		//// Very bad way to generate a random color
		//ParticlesContainer[particleIndex].r = rand() % 256;
		//ParticlesContainer[particleIndex].g = rand() % 256;
		//ParticlesContainer[particleIndex].b = rand() % 256;
		//ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		//ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
		ParticlesContainer[particleIndex].create(maindir, spread);
	//--
	}

	// Simulate all particles
	int ParticlesCount = 0;

	for (int i = 0; i<MaxParticles; i++) {

		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.isDead() == false) {

			p.decrease(delta, ParticlesCount, g_particule_position_size_data, g_particule_color_data, CameraPosition);
			//if (p.life > 0.0f) {

			//	// Decrease life
			//	p.life -= delta;
			//	if (p.life > 0.0f) {

			//		// Simulate simple physics : gravity only, no collisions
			//		p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
			//		p.pos += p.speed * (float)delta;
			//		p.cameradistance = glm::length2(p.pos - CameraPosition);
			//		//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

			//		// Fill the GPU buffer
			//		g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
			//		g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
			//		g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

			//		g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

			//		g_particule_color_data[4 * ParticlesCount + 0] = p.r;
			//		g_particule_color_data[4 * ParticlesCount + 1] = p.g;
			//		g_particule_color_data[4 * ParticlesCount + 2] = p.b;
			//		g_particule_color_data[4 * ParticlesCount + 3] = p.a;

			//	} else {
			//		// Particles that just died will be put at the end of the buffer in SortParticles();
			//		p.cameradistance = -1.0f;
			//	}

			//	ParticlesCount++;

			//}

			ParticlesCount++;
		}
	}

	SortParticles();

	//printf("%d ",ParticlesCount);


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

	//ParticleEmitter::ParticleEmitter (Node* _parent, std::string _name ) : Draw (_parent, EntityKind::PARTICLE_SYSTEM, _name ) {
//
//    coresPart.push_back ( Color ( 1.0f, 0.5f, 0.5f ) );
//    coresPart.push_back ( Color ( 1.0f, 0.75f, 0.5f ) );
//    coresPart.push_back ( Color ( 1.0f, 1.0f, 0.5f ) );
//    coresPart.push_back ( Color ( 0.75f, 1.0f, 0.5f ) );
//    coresPart.push_back ( Color ( 0.5f, 1.0f, 0.5f ) );
//    coresPart.push_back ( Color ( 0.5f, 1.0f, 0.75f ) );
//    coresPart.push_back ( Color ( 0.5f, 1.0f, 1.0f ) );
//    coresPart.push_back ( Color ( 0.5f, 0.75f, 1.0f ) );
//    coresPart.push_back ( Color ( 0.75f, 0.5f, 1.0f ) );
//    coresPart.push_back ( Color ( 1.0f, 0.5f, 1.0f ) );
//    coresPart.push_back ( Color ( 1.0f, 0.5f, 0.75f ) );
//}
//
//ParticleEmitter::~ParticleEmitter() {
//}
//
//void ParticleEmitter::init() {
//	
//	particles.reserve(maxSeed);
//
//	// Reset all the particles
//	for (int loop = 0; loop < maxSeed; loop++) {
//
//		int indiceCor = loop % coresPart.size();                 // (loop + 1) / (_max / coresPart.size());
//
//		glm::vec3 direcao(  (float)((rand() % 50) - 26.0f) * 10.0f,
//                            (float)((rand() % 50) - 25.0f) * 10.0f,
//                            (float)((rand() % 50) - 25.0f) * 10.0f);   // yi e zi repetiam no original
//
//		Particle *pParticle = new Particle(&posSource, direcao, glm::vec3(0.0, 0.0, 0.0), coresPart[indiceCor], 10.0f);
//		particles.push_back(pParticle);
//	}
//
//	pState->init();
//}
//
//void ParticleEmitter::setSizeBox ( const glm::vec3& _size ) {
//    sizeBox = _size;
//}
//
//glm::vec3 ParticleEmitter::getSizeBox() {
//    return sizeBox;
//}
//
//void ParticleEmitter::accept(NodeVisitor * v) {
//	v->visit(this);
//}
//
//void ParticleEmitter::setGL() {
//
//    // Enable smooth shading
//    glShadeModel ( GL_SMOOTH );
//
//    // Set the background black
//    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//    // Depth buffer setup
//    //glClearDepth(1.0f);
//
//    // Enables Depth Testing
//    //glDisable(GL_DEPTH_TEST);
//
//    // Enable Blending
//    glEnable ( GL_BLEND );
//
//    // Type Of Blending To Perform
//    glBlendFunc ( GL_SRC_ALPHA, GL_ONE );
//    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    // Really Nice Perspective Calculations
//    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//
//    // Really Nice Point Smoothing
//    glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
//
//    // Enable Texture Mapping
//    glEnable ( GL_TEXTURE_2D );
//
//    glDisable ( GL_LIGHTING );
//
//}
//
//void ParticleEmitter::loadImage ( const char *_file ) {
//
//	Texture *pTex = new Texture("TexParticleEmmiter_" + std::to_string(getSerial()), std::string(_file));
//
//	Material *pMat = new Material("MatParticleEmmiter_" + std::to_string(getSerial()));
//	pMat->setTexDiffuse(pTex);
//
//	pState->setMaterial(pMat);
//	
//}
//
//void ParticleEmitter::renderExecute(bool _texture) {
//
//    glPushAttrib ( GL_ENABLE_BIT );
//    glPushAttrib ( GL_CURRENT_BIT );
//
//    setGL();
//
//    // desloca objeto em relacao ao que contem a camera
//    glTranslatef ( -position.x , -position.y , -position.z );
//
//    // armazena a camera para uso no ordenador //TODO: Otimizar criando lista de cameras no Object e indicando qual esta ativa neste momento
//    Camera *pCam = (Camera*)Node::findNodeBySeq(EntityKind::CAMERA, 0);//FIXME!!!!!! //pSource->findChildByKind ( EntityKind::CAMERA, 0 );
//    if ( pCam !=  nullptr ) {
//        SortParticles ( pCam->getPosition() );
//    }
//    
//    // Select Our Texture
//	//if (pState->getTexture() != nullptr)
//	//	pState->appyTexture();
//	//else
//	//	glBindTexture(GL_TEXTURE_2D, 0);
//	pState->appyMaterial(nullptr);
//
//    for ( unsigned loop = 0; loop < particles.size(); loop++ ) {
//        Particle *pParticle = particles[loop];
//        pParticle->render();
//    }
//    
//    glPopAttrib();
//    glPopAttrib();
//       
//}
//
//// estrutura de comparacao
//// TODO Otimizar
//struct ParticleCompare {
//    const glm::vec3 *pPosicao;
//    bool operator() ( Particle* i,Particle* j ) {
//
//        bool retorno;
//                
//        float d1 = glm::distance(*pPosicao , i->position); //pPosicao->distance ( i->position );
//        float d2 = glm::distance(*pPosicao , j->position); //pPosicao->distance ( j->position );
//        retorno = ( d2 < d1 );
//
//        return retorno;
//    }
//};
//
//
//void ParticleEmitter::SortParticles ( const glm::vec3 &posCamera ) {
//
////// Lambda nao carrega o this
//// 		std::sort(particles.begin(), particles.end(), [](Particle* a, Particle* b){
////
//// 			bool ret;
//// 			float d1 = posCamera.distance(a->position);
//// 			float d2 = posCamera.distance(b->position);
////
//// 			ret = d1 < d2;
////
//// 			return ret;
//// 		});
//
//
//    static ParticleCompare comparador;
//    comparador.pPosicao = &posCamera;
//    std::sort ( particles.begin(), particles.end(), comparador );
//
//}
}