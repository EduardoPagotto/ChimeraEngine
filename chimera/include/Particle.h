#ifndef _PARTICLE__H
#define _PARTICLE__H

#include "Color.h"
#include "glm/glm.hpp"

#include "OpenGLDefs.h"

namespace Chimera
{

class Particle {
public:

	Particle();
	virtual ~Particle();

	void create(const glm::vec3 &_maindir, const float &_spread);

	void decrease(const float &_delta, int _particlesCount, GLfloat*  pPositionSizeData, GLubyte* pColorData, const glm::vec3 &_cameraPosition);

	void reset();

	bool isDead() {
		return (life < 0);
	}

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}

private:

	glm::vec3 pos, speed;
	Color color; //unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

};



//class Particle
//{
//public:
//    Particle ( glm::vec3 *pInitPosition, const glm::vec3& direction, const glm::vec3& gravity , const Color& color,const float& live );
//    virtual ~Particle();
//
//    void ResetParticle ( Color color, const glm::vec3 &dir );
//
//    void render();
//
//    int   active; /* Active (Yes/No) */
//    float life;   /* Particle Life   */
//
//    bool operator < ( const Particle& that ) const;
//
//    Color corAtiva;
//
//    glm::vec3 position;
//    glm::vec3 direction;
//    glm::vec3 gravity;
//
//private:
//    glm::vec3 *initPosition;
//    float initLive;
//    void camera_directions ( glm::vec3 *right, glm::vec3 *up, glm::vec3 *look );
//    void draw_billboard ( const glm::vec3 &posicao );
//
//};
}
#endif