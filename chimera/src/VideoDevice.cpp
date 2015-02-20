#include "VideoDevice.h"

#include "ExceptionSDL.h"

#ifdef WIN32
#include "windows.h"
#endif

//#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

	VideoDevice::VideoDevice(std::string _nome) : Video(_nome, KIND_DEVICE::SCREEN) {

		fullscreenStatus = false;

		//chama inicializacao do SDL na classe Pai
		initSDL();

	}

	VideoDevice::VideoDevice(int _width, int _height, std::string _nome) : Video(_nome, KIND_DEVICE::SCREEN, _width, _height) {

		fullscreenStatus = false;

		//chama inicializacao do SDL na classe Pai
		initSDL();
	}

	VideoDevice::~VideoDevice() {

	}

	void VideoDevice::initDraw(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VideoDevice::endDraw() {
		SDL_GL_SwapWindow(window);
	}

	void VideoDevice::executeViewPerspective(Camera *pCamera, int _eye) {

		glViewport(0, 0, winSizeW, winSizeH);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(pCamera->getFov(), (GLfloat)(float)winSizeW / (float)winSizeH, pCamera->getNear(), pCamera->getFar());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}

	void VideoDevice::executeViewOrto(int eye) {

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, winSizeW, 0, winSizeH, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

	}

	void VideoDevice::reshape(int _w, int _h) {
		winSizeW = _w;
		winSizeH = _h;
	}

	void VideoDevice::toggleFullScreen() {

		if (fullscreenStatus == false) {

			SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);
			//SDL_GetWindowSize(window, &winGeometryPrev.w, &winGeometryPrev.h);

			SDL_SetWindowPosition(window, 0, 0);
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		}
		else {

			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);
			//SDL_SetWindowSize(window, winGeometryPrev.w, winGeometryPrev.h);

		}

		fullscreenStatus = !fullscreenStatus;
	}
}