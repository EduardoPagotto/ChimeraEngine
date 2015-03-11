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

	    //friend bool operator< (Particle &val1, Particle &val2);
		//bool operator<(Particle& that);
		bool operator < (const Particle& that) const;

		Color corAtiva;


		btVector3 position;
		//float x;      /* X Position      */
		//float y;      /* Y Position      */
		//float z;      /* Z Position      */

	 	btVector3 direction;
		//float xi;     /* X Direction     */
		//float yi;     /* Y Direction     */
		//float zi;     /* Z Direction     */

		btVector3 gravity;
		//float xg;     /* X Gravity       */
		//float yg;     /* Y Gravity       */
		//float zg;     /* Z Gravity       */

	};
}
#endif