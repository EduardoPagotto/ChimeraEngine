#ifndef _PARTICLE__H
#define _PARTICLE__H

#include "Color.h"

namespace Chimera {

	class Particle {
	public:

		Particle();
		virtual ~Particle();

		void ResetParticle(Color color, float xDir, float yDir, float zDir);

		void render();

		int   active; /* Active (Yes/No) */
		float life;   /* Particle Life   */

		Color corAtiva;



		float x;      /* X Position      */
		float y;      /* Y Position      */
		float z;      /* Z Position      */

		float xi;     /* X Direction     */
		float yi;     /* Y Direction     */
		float zi;     /* Z Direction     */

		float xg;     /* X Gravity       */
		float yg;     /* Y Gravity       */
		float zg;     /* Z Gravity       */

	};
}
#endif