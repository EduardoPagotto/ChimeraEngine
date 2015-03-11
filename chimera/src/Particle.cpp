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

		life = 5.0f; // Give the particles life

		corAtiva = color;
		corAtiva.a = (float)(rand() % 100) / 1000.0f + 0.003f; 	//A como fade!

		position.setZero();
		position.setX(1.0f);
		position.setY(-1.0f);
		position.setZ(-1.0f);

		direction = dir;                                    	// Random Speed On X,Y,Z Axis
		gravity.setValue(0.0f, -0.0f, 0.5f);                   // Set Horizontal Pull To Zero

	}


	static void camera_directions(
		float * out_rgt,		                                    // Any can be NULL
		float * out_up,
		float * out_look)
	{
		float m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);

		// Roughly speaking, a modelview matrix is made up more or less like this:
		// [ EyeX_x EyeX_y EyeX_z    a
		//   EyeY_x EyeY_y EyeY_z    b
		//   EyeZ_x EyeZ_y EyeZ_z    c
		//   um don't look down here ]
		// where a, b, c are translations in _eye_ space.  (For this reason, a,b,c is not
		// the camera location - sorry!)

		if (out_rgt) {
			out_rgt[0] = m[0];
			out_rgt[1] = m[4];
			out_rgt[2] = m[8];
		}
		if (out_up) {
			out_up[0] = m[1];
			out_up[1] = m[5];
			out_up[2] = m[9];
		}
		if (out_look) {
			out_up[0] = m[2];
			out_up[1] = m[6];
			out_up[2] = m[10];
		}
	}

	static void draw_billboard(float x, float y, float z)
	{

		float r[3], u[3];
		camera_directions(r, u, NULL);
		glBegin(GL_TRIANGLE_STRIP);

		float xf, yf, zf = 0.0;

		xf = x + r[0] + u[0];
		yf = y + r[1] + u[1];
		zf = z + r[2] + u[2];
		glTexCoord2d(1, 1);
		glVertex3f(x + r[0] + u[0], y + r[1] + u[1], z + r[2] + u[2]);

		xf = x - r[0] + u[0];
		yf = y - r[1] + u[1];
		zf = z - r[2] + u[2];
		glTexCoord2d(0, 1);
		glVertex3f(x - r[0] + u[0], y - r[1] + u[1], z - r[2] + u[2]);

		xf = x + r[0] - u[0];
		yf = y + r[1] - u[1];
		zf = z + r[2] - u[2];
		glTexCoord2d(1, 0);
		glVertex3f(x + r[0] - u[0], y + r[1] - u[1], z + r[2] - u[2]);

		xf = x - r[0] - u[0];
		yf = y - r[1] - u[1];
		zf = z - r[2] - u[2];
		glTexCoord2d(0, 0);
		glVertex3f(x - r[0] - u[0], y - r[1] - u[1], z - r[2] - u[2]);

		glEnd();
	}

	bool Particle::operator<(Particle& that) {
		// Sort in reverse order : far particles drawn first.
		//return this->cameradistance > that.cameradistance;
		return false;
	}

	void Particle::render() {

		GLuint col = 0;        	// Current Color Selection
		float slowdown = 2.0f; 	// Slow Down Particles                                
		float zoom = -40.0f;	// Used To Zoom Out                                   
		float xspeed = 0;		// Base X Speed (To Allow Keyboard Direction Of Tail) 
		float yspeed = 0;		// Base Y Speed (To Allow Keyboard Direction Of Tail) 

		if (active)
		{
			// Draw The Particle Using Our RGB Values,
			// Fade The Particle Based On It's Life
			glColor4f(corAtiva.r, corAtiva.g, corAtiva.b, life);//life in alpha

			draw_billboard(position.getX(), position.getY(), position.getZ());

			// Move On The X Axis By X,Y,Z Speed 
			position += direction * (1 / (slowdown * 1000));

			// Take Pull On X,Y,Z Axis Into Account 
			direction += gravity;

			// Reduce Particles Life By 'Fade' 
			life -= corAtiva.a;

			// If the particle dies, revive it 
			if (life < 0.0f)
			{
				btVector3 novo(xspeed + (float)((rand() % 60) - 32.0f),
					yspeed + (float)((rand() % 60) - 30.0f),
					(float)((rand() % 60) - 30.0f));

				ResetParticle(corAtiva, novo); //FIXME: mudar a cor???
			}
		}
	}

	//FIXME remover depois de acertar o tamanho do billboard
	//void Particle::render() {
	//	GLuint col = 0;        	/* Current Color Selection                            */
	//	float slowdown = 2.0f; 	/* Slow Down Particles                                */
	//	float zoom = -40.0f;	/* Used To Zoom Out                                   */
	//	float xspeed = 0;		/* Base X Speed (To Allow Keyboard Direction Of Tail) */
	//	float yspeed = 0;		/* Base Y Speed (To Allow Keyboard Direction Of Tail) */

	//	if (active)
	//	{
	//		/* Draw The Particle Using Our RGB Values,
	//		 * Fade The Particle Based On It's Life
	//		 */
	//		glColor4f(corAtiva.r, corAtiva.g, corAtiva.b, life);     //life in alpha

	//		draw_billboard(position.getX(), position.getY(), position.getZ());


	//		// 	 // Build Quad From A Triangle Strip
	//		// 	 glBegin(GL_TRIANGLE_STRIP);
	//		// 	 // Top Right
	//		// 	 glTexCoord2d(1, 1);
	//		// 	 glVertex3f(position.getX() + 0.5f, position.getY() + 0.5f, position.getZ());
	//		// 	 // Top Left
	//		// 	 glTexCoord2d(0, 1);
	//		// 	 glVertex3f(position.getX() - 0.5f, position.getY() + 0.5f, position.getZ());
	//		// 	 // Bottom Right
	//		// 	 glTexCoord2d(1, 0);
	//		// 	 glVertex3f(position.getX() + 0.5f, position.getY() - 0.5f, position.getZ());
	//		// 	 // Bottom Left
	//		// 	 glTexCoord2d(0, 0);
	//		// 	 glVertex3f(position.getX() - 0.5f, position.getY() - 0.5f, position.getZ());
	//		// 	 glEnd();

	//		/* Move On The X Axis By X,Y,Z Speed */
	//		//position.setX( position.getX() + ( direction.getX() / (slowdown * 1000)));
	//		//position.setY( position.getY() + ( direction.getY() / (slowdown * 1000)));
	//		//position.setZ( position.getZ() + ( direction.getZ() / (slowdown * 1000)));

	//		position += direction * (1 / (slowdown * 1000));

	//		//x += xi / (slowdown * 1000);
	//		//y += yi / (slowdown * 1000);
	//		//z += zi / (slowdown * 1000);

	//		/* Take Pull On X,Y,Z Axis Into Account */
	//		//xi += xg;
	//		//yi += yg;
	//		//zi += zg;
	//		direction += gravity;

	//		/* Reduce Particles Life By 'Fade' */
	//		//life -= fade;
	//		life -= corAtiva.a;

	//		/* If the particle dies, revive it */
	//		if (life < 0.0f)
	//		{
	//			//float xi, yi, zi;
	//			//xi = xspeed + (float)((rand() % 60) - 32.0f);
	//			//yi = yspeed + (float)((rand() % 60) - 30.0f);
	//			//zi = (float)((rand() % 60) - 30.0f);

	//			btVector3 novo(xspeed + (float)((rand() % 60) - 32.0f),
	//				yspeed + (float)((rand() % 60) - 30.0f),
	//				(float)((rand() % 60) - 30.0f));


	//			ResetParticle(corAtiva, novo);                       //FIXME: mudar a cor???
	//		}
	//	}
	//}
}
