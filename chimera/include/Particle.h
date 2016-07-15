#ifndef _PARTICLE__H
#define _PARTICLE__H

# include <LinearMath/btVector3.h>
#include "Color.h"

namespace Chimera
{

class Particle
{
public:
    Particle ( btVector3 *pInitPosition, const btVector3& direction, const btVector3& gravity , const Color& color,const float& live );
    virtual ~Particle();

    void ResetParticle ( Color color, const btVector3 &dir );

    void render();

    int   active; /* Active (Yes/No) */
    float life;   /* Particle Life   */

    bool operator < ( const Particle& that ) const;

    Color corAtiva;

    btVector3 position;
    btVector3 direction;
    btVector3 gravity;

private:
    btVector3 *initPosition;
    float initLive;
    void camera_directions ( btVector3 *right, btVector3 *up, btVector3 *look );
    void draw_billboard ( const btVector3 &posicao );

};
}
#endif