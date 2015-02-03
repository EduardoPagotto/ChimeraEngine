#include "OvrDevice.h"
#include "ExceptionSDL.h"

#include "../Src/OVR_CAPI_GL.h"

#ifdef WIN32
#include "windows.h"
#endif

namespace Chimera {

OvrDevice::OvrDevice(std::string nomeTela){

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		int x = SDL_WINDOWPOS_CENTERED;
		int y = SDL_WINDOWPOS_CENTERED;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		bool debug = false;

		ovr_Initialize();

		hmd = ovrHmd_Create(0);
		if (hmd == NULL) {
			hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
			debug = true;
		}

		if (debug == false) {
			x = hmd->WindowsPos.x;
			y = hmd->WindowsPos.y;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		int w = hmd->Resolution.w;
		int h = hmd->Resolution.h;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		window = SDL_CreateWindow(nomeTela.c_str(),	//    window title
			x,										//    initial x position
			y,										//    initial y position
			w,										//    width, in pixels
			y,										//    height, in pixels
			flags);

		if (window != nullptr) {

			context = SDL_GL_CreateContext(window);
			if (context != nullptr) {

				initOVRSubSys();

				return;
			}
		}
	}

	throw ExceptionSDL(ExceptionCode::CREATE, std::string(SDL_GetError()));
}

OvrDevice::~OvrDevice(){

}

void OvrDevice::initOVRSubSys(){

	using namespace OVR;

	Sizei recommendedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
	Sizei recommendedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);
	Sizei renderTargetSize;
	renderTargetSize.w = recommendedTex0Size.w + recommendedTex1Size.w;
	renderTargetSize.h = max(recommendedTex0Size.h, recommendedTex1Size.h);


	ovrFovPort eyeFov[2] = { hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1] };

	ovrRecti eyeRenderViewport[2];
	eyeRenderViewport[0].Pos = Vector2i(0, 0);
	eyeRenderViewport[0].Size = Sizei(renderTargetSize.w / 2, renderTargetSize.h);
	eyeRenderViewport[1].Pos = Vector2i((renderTargetSize.w + 1) / 2, 0);
	eyeRenderViewport[1].Size = eyeRenderViewport[0].Size;

	ovrGLTexture eyeTexture[2];
	eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	eyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
	eyeTexture[0].OGL.Header.RenderViewport = eyeRenderViewport[0];
	eyeTexture[0].OGL.TexId = texture;

	eyeTexture[1] = eyeTexture[0];
	eyeTexture[1].OGL.Header.RenderViewport = eyeRenderViewport[1];

}

}

