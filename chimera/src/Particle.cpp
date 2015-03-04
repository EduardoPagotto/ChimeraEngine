#include "Particle.h"

#include <stdlib.h>

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	Particle::Particle() {
	}

	Particle::~Particle() {

	}


	void Particle::ResetParticle(Color color, const btVector3 &dir)
	{

		active = true;	//Make the particels active

		life = 1.0f; // Give the particles life

		corAtiva = color;
	    corAtiva.a = (float)(rand() % 100) / 1000.0f + 0.003f; 	//A como fade!

		position.setZero();

	 	direction = dir;                                    	// Random Speed On X,Y,Z Axis
	 	gravity.setValue(0.0f, -0.8f,  0.0f);                   // Set Horizontal Pull To Zero

	}

	void Particle::render() {

		GLuint col = 0;        	/* Current Color Selection                            */
		float slowdown = 2.0f; 	/* Slow Down Particles                                */
		float zoom = -40.0f;	/* Used To Zoom Out                                   */
		float xspeed = 0;		/* Base X Speed (To Allow Keyboard Direction Of Tail) */
		float yspeed = 0;		/* Base Y Speed (To Allow Keyboard Direction Of Tail) */

		if (active)
		{
			/* Draw The Particle Using Our RGB Values,
			* Fade The Particle Based On It's Life
			*/
			glColor4f(corAtiva.r, corAtiva.g, corAtiva.b, life);//life in alpha

			/* Build Quad From A Triangle Strip */
			glBegin(GL_TRIANGLE_STRIP);
			/* Top Right */
			glTexCoord2d(1, 1);
			glVertex3f(position.getX() + 0.5f, position.getY() + 0.5f, position.getZ());
			/* Top Left */
			glTexCoord2d(0, 1);
	  		glVertex3f(position.getX() - 0.5f, position.getY() + 0.5f, position.getZ());
			/* Bottom Right */
			glTexCoord2d(1, 0);
	  		glVertex3f(position.getX() + 0.5f, position.getY() - 0.5f, position.getZ());
			/* Bottom Left */
			glTexCoord2d(0, 0);
	  		glVertex3f(position.getX() - 0.5f, position.getY() - 0.5f, position.getZ());
			glEnd();

			/* Move On The X Axis By X,Y,Z Speed */
			//position.setX( position.getX() + ( direction.getX() / (slowdown * 1000)));
			//position.setY( position.getY() + ( direction.getY() / (slowdown * 1000)));
			//position.setZ( position.getZ() + ( direction.getZ() / (slowdown * 1000)));

	  		position += direction * ( 1 / (slowdown * 1000));

			//x += xi / (slowdown * 1000);
			//y += yi / (slowdown * 1000);
			//z += zi / (slowdown * 1000);

			/* Take Pull On X,Y,Z Axis Into Account */
			//xi += xg;
			//yi += yg;
			//zi += zg;
			direction += gravity;

			/* Reduce Particles Life By 'Fade' */
			//life -= fade;
			life -= corAtiva.a;

			/* If the particle dies, revive it */
			if (life < 0.0f)
			{
				//float xi, yi, zi;
				//xi = xspeed + (float)((rand() % 60) - 32.0f);
				//yi = yspeed + (float)((rand() % 60) - 30.0f);
				//zi = (float)((rand() % 60) - 30.0f);

				btVector3 novo(xspeed + (float)((rand() % 60) - 32.0f),
								yspeed + (float)((rand() % 60) - 30.0f),
								(float)((rand() % 60) - 30.0f));


	   			ResetParticle(corAtiva, novo); //FIXME: mudar a cor???
			}
		}
	}
}
