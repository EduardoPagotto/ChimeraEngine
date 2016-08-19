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
	Color color; 
	float size, angle, weight;
	float life; 
	float cameradistance;
};
}
#endif