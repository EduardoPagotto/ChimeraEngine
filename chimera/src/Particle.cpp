#include "Particle.h"

#include <stdlib.h>

#include <OpenGLDefs.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

namespace Chimera {
Particle::Particle() {

	life = -1.0f;
	cameradistance = -1.0f;

}
Particle::~Particle()
{
}

void Particle::create(const glm::vec3 & _maindir, const float &_spread)
{
	// Very bad way to generate a random direction; 
	// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
	// combined with some user-controlled parameters (main direction, spread, etc)
	glm::vec3 randomdir = glm::vec3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);

	speed = _maindir + randomdir * _spread;

	// Very bad way to generate a random color
	color.set(rand() % 256, rand() % 256, rand() % 256, rand() % 256 / 3); //FIXME acho que esta errado, conferir em debug
	size = (rand() % 1000) / 2000.0f + 0.1f;
}

void Particle::decrease(const float &_delta, int _particlesCount, GLfloat*  pPositionSizeData, GLubyte* pColorData, const glm::vec3 &_cameraPosition)
{
	// Decrease life
	life -= _delta;
	if (life > 0.0f) {

		// Simulate simple physics : gravity only, no collisions
		speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)_delta * 0.00000001f;
		pos += speed * (float)_delta;
		cameradistance = glm::length2(pos - _cameraPosition);
		//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

		// Fill the GPU buffer
		pPositionSizeData[4 * _particlesCount + 0] = pos.x;
		pPositionSizeData[4 * _particlesCount + 1] = pos.y;
		pPositionSizeData[4 * _particlesCount + 2] = pos.z;

		pPositionSizeData[4 * _particlesCount + 3] = size;

		pColorData[4 * _particlesCount + 0] = color.r;// p.r;
		pColorData[4 * _particlesCount + 1] = color.g;// p.g;
		pColorData[4 * _particlesCount + 2] = color.b;// p.b;
		pColorData[4 * _particlesCount + 3] = color.a;//p.a;

	} else {
		// Particles that just died will be put at the end of the buffer in SortParticles();
		cameradistance = -1.0f;
	}

}

void Particle::reset()
{
	life = 500.0f; // This particle will live 5 seconds.
	pos = glm::vec3(0, 0, -20.0f);
}


	//Particle::Particle (glm::vec3 *pInitPosition, const glm::vec3& direction, const glm::vec3& gravity , const Color& color,const float& live ) {
//
//    this->initPosition = pInitPosition;
//    this->initLive = live;
//    this->gravity  = gravity;
//    ResetParticle ( color, direction );
//}
//
//Particle::~Particle() {
//}
//
//void Particle::ResetParticle ( Color color, const glm::vec3 &dir ) {
//    active = true;	//Make the particels active
//    life = initLive;// Give the particles life
//
//    corAtiva = color;
//    corAtiva.a = ( float ) ( rand() % 100 ) / 1000.0f + 0.003f; 	//A como fade!
//
//    position = *initPosition;
//    direction = dir;                                    	// Random Speed On X,Y,Z Axis
//
//}
//
//// Roughly speaking, a modelview matrix is made up more or less like this:
//// [ EyeX_x EyeX_y EyeX_z    a
////   EyeY_x EyeY_y EyeY_z    b
////   EyeZ_x EyeZ_y EyeZ_z    c
////   um don't look down here ]
//// where a, b, c are translations in _eye_ space.  (For this reason, a,b,c is not
//// the camera location - sorry!)
//void Particle::camera_directions ( glm::vec3 *right, glm::vec3 *up, glm::vec3 *look ) {
//    float m[16];
//    glGetFloatv ( GL_MODELVIEW_MATRIX, m );
//
//    if ( right != nullptr ) {
//        *right = glm::vec3( m[0], m[4], m[8] );
//    }
//
//    if ( up != nullptr ) {
//        *up = glm::vec3( m[1], m[5], m[9] );
//    }
//
//    if ( look != nullptr ) {
//        *look = glm::vec3( m[2], m[6], m[10] );
//    }
//
//}
//
//void Particle::draw_billboard ( const glm::vec3 &posicao ) {
//    glm::vec3 right;
//    glm::vec3 up;
//
//    camera_directions ( &right, &up, nullptr );
//    glBegin ( GL_TRIANGLE_STRIP );
//
//    float ks = 5.0f;
//    right /= ks;
//    up /= ks;
//
//    glm::vec3 final = posicao + right + up;          // right-up
//    glTexCoord2d ( 1, 1 );
//    glVertex3fv ( glm::value_ptr(final));                        //   final.x(), final.y(),  final.z());
//
//    final = posicao - right + up;                     // Left-up
//    glTexCoord2d ( 0, 1 );
//    glVertex3fv ( glm::value_ptr(final) );
//
//    final = posicao + right - up;                      // right-bottom
//    glTexCoord2d ( 1, 0 );
//    glVertex3fv ( glm::value_ptr(final) );
//
//    final = posicao - right - up;                       // left-bottom
//    glTexCoord2d ( 0, 0 );
//    glVertex3fv ( glm::value_ptr(final) );
//
//    glEnd();
//}
//
//void Particle::render() {
//
//    GLuint col = 0;        	// Current Color Selection
//    float slowdown = 2.0f; 	// Slow Down Particles
//    float zoom = -40.0f;	// Used To Zoom Out
//    float xspeed = 0;		// Base X Speed (To Allow Keyboard Direction Of Tail)
//    float yspeed = 0;		// Base Y Speed (To Allow Keyboard Direction Of Tail)
//
//    if ( active ) {
//        // Draw The Particle Using Our RGB Values,
//        // Fade The Particle Based On It's Life
//        glColor4f ( corAtiva.r, corAtiva.g, corAtiva.b, life ); //life in alpha
//
//        draw_billboard ( position );
//
//        // Move On The X Axis By X,Y,Z Speed
//        position += direction * ( 1 / ( slowdown * 1000 ) );
//
//        // Take Pull On X,Y,Z Axis Into Account
//        direction += gravity;
//
//        // Reduce Particles Life By 'Fade'
//        life -= corAtiva.a;
//
//        // If the particle dies, revive it
//        if ( life < 0.0f ) {
//            glm::vec3 novaDirecao ( xspeed + ( float ) ( ( rand() % 60 ) - 32.0f ),
//                                    yspeed + ( float ) ( ( rand() % 60 ) - 30.0f ),
//                                    ( float ) ( ( rand() % 60 ) - 30.0f ) );
//
//            ResetParticle ( corAtiva, novaDirecao );                //FIXME: mudar a cor???
//        }
//    }
//}
}

