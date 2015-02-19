#ifndef OVR_DEVICE_H_
#define OVR_DEVICE_H_

#define GLEW_STATIC

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
#define OVR_OS_WIN32
#include "../Src/OVR_CAPI_GL.h"
#include "OVR.h"
#else
#define OVR_OS_LINUX
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "Include/OVR.h"
#include "Src/OVR_CAPI_GL.h"
#include <algorithm>
#endif

// #ifdef WIN32
// #include "../Src/OVR_CAPI_GL.h"
// #else
// #include "Src/OVR_CAPI_GL.h"
// #include <algorithm>
// #endif


namespace Chimera {

	class OvrDevice : public Video {
	public:
		OvrDevice(std::string nomeTela);
		virtual ~OvrDevice();

		void initGL();
		void initDraw();
		void endDraw();
		void getGeometry(int &_x, int &_y, int &_w, int &_h, int index);

		virtual void executeViewPerspective(Camera *pCamera, int _eye);
		virtual void executeViewOrto(int eyeIndex);

	private:
		void initDevice();
		virtual void reshape(int x, int y);
		void update_rtarg(int width, int height);
		unsigned int next_pow2(unsigned int x);
		void quat_to_matrix(const float *quat, float *mat);

		virtual void toggleFullScreen();
		
		void initOVRSubSys();

		void openFrameBuffer();
		void closeFrameBuffer();

		ovrHmd hmd;
		//ovrRecti eyeRenderViewport[2];
		//ovrPosef eyeRenderPose[2];
		//ovrEyeRenderDesc eyeRenderDesc[2];
		//ovrGLTexture eyeTexture[2];
		//OVR::Sizei renderTargetSize;

		SDL_Window *window;
		SDL_GLContext context;

		unsigned int fbo, fb_tex, fb_depth;
		int fb_tex_width, fb_tex_height;
		ovrGLTexture fb_ovr_tex[2];
		union ovrGLConfig glcfg;
		unsigned int hmd_caps;
		unsigned int distort_caps;
		ovrEyeRenderDesc eye_rdesc[2];
		ovrPosef pose[2];
		int win_width;
		int win_height;
		int fb_width, fb_height;

		//GLuint frameBufferObject;
		//GLuint renderBuffer;
		//GLuint texture;
	};


}

#endif

