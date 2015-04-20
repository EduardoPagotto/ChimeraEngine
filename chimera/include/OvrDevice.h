#ifndef OVR_DEVICE_H_
#define OVR_DEVICE_H_

#include "Video.h"

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

namespace Chimera {

	class OvrDevice : public Video {
	public:
		OvrDevice(std::string nomeTela);
		virtual ~OvrDevice();

		virtual void initDraw();
		virtual void endDraw();
		virtual void executeViewPerspective(const float &_fov,const float &_near,const float &_far, int _eye);
		virtual void executeViewOrto(int eyeIndex);
		virtual void reshape(int _w, int _h);
		virtual void toggleFullScreen();

	private:
		void initDevice();
		void update_rtarg(int width, int height);
		void quat_to_matrix(const float *quat, float *mat);
		unsigned int next_pow2(unsigned int x);

		unsigned int fbo;
		unsigned int fb_tex;
		unsigned int fb_depth;
		unsigned int hmd_caps;
		unsigned int distort_caps;

		union ovrGLConfig glcfg;
		ovrSizei fbSize;
		ovrSizei fbTexSize;
		ovrHmd hmd;
		ovrGLTexture fb_ovr_tex[2];
		ovrEyeRenderDesc eye_rdesc[2];
		ovrPosef pose[2];

		bool fullscreenStatus;
		ovrVector2i winPosPrev;


		ovrMatrix4f orthoProjection[2];
	};


}

#endif

