#ifndef _PARTICLE__H
#define _PARTICLE__H

# include <LinearMath/btVector3.h>
#include "Color.h"

namespace Chimera {

	class Particle {
	public:

		Particle();
		virtual ~Particle();

	    void ResetParticle(Color color, const btVector3 &dir);

		void render();

		int   active; /* Active (Yes/No) */
		float life;   /* Particle Life   */

		bool operator < (const Particle& that) const;

		Color corAtiva;

		btVector3 position;
	 	btVector3 direction;
		btVector3 gravity;
	};
}
#endif