#ifndef OVR_DEVICE_H_
#define OVR_DEVICE_H_

#include "Video.h"

#ifdef WIN32
#include "windows.h"
#endif

#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <string>

#ifdef WIN32
#include "OVR.h"
#else
#include "Include/OVR.h"
#endif

namespace Chimera {

class OvrDevice : public Video {
public:
	OvrDevice(std::string nomeTela);
	virtual ~OvrDevice();

    void initGL ();
    void initDraw();
    void endDraw();
    void getGeometry(int &_x, int &_y, int &_w, int &_h, int index);

    virtual void executeViewPerspective(Camera *pCamera, int _eye);

private:
	void initOVRSubSys();

    void openFrameBuffer();
    void closeFrameBuffer();

	ovrHmd hmd;

	//SDL_Rect rectangle;
    OVR::Sizei renderTargetSize;
    ovrRecti eyeRenderViewport[2];
    ovrPosef eyeRenderPose[2];

	SDL_Window *window;
	SDL_GLContext context;

    GLuint frameBufferObject;
    GLuint renderBuffer;
    GLuint texture;
};


}

#endif

