#ifndef _PARTICLE__H
#define _PARTICLE__H

#include "Color.h"
#include "glm/glm.hpp"

namespace Chimera
{

class Particle
{
public:
    Particle ( glm::vec3 *pInitPosition, const glm::vec3& direction, const glm::vec3& gravity , const Color& color,const float& live );
    virtual ~Particle();

    void ResetParticle ( Color color, const glm::vec3 &dir );

    void render();

    int   active; /* Active (Yes/No) */
    float life;   /* Particle Life   */

    bool operator < ( const Particle& that ) const;

    Color corAtiva;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 gravity;

private:
    glm::vec3 *initPosition;
    float initLive;
    void camera_directions ( glm::vec3 *right, glm::vec3 *up, glm::vec3 *look );
    void draw_billboard ( const glm::vec3 &posicao );

};
}
#endif