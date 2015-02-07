#include "OvrDevice.h"
#include "ExceptionSDL.h"

#ifdef WIN32
#include "../Src/OVR_CAPI_GL.h"
#else
#include "Src/OVR_CAPI_GL.h"
#include <algorithm>
#endif

#include <SDL_opengl.h>
#include <SDL_syswm.h>

namespace Chimera {

OvrDevice::OvrDevice(std::string nomeTela) : Video(nomeTela) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		int x = 600;//SDL_WINDOWPOS_CENTERED;
		int y = 600;//SDL_WINDOWPOS_CENTERED;
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

				using namespace OVR;

				Sizei recommendedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
				Sizei recommendedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);
				//Sizei renderTargetSize;
				renderTargetSize.w = recommendedTex0Size.w + recommendedTex1Size.w;

				#ifdef WIN32
				renderTargetSize.h = max(recommendedTex0Size.h, recommendedTex1Size.h);
				#else
				renderTargetSize.h = std::max(recommendedTex0Size.h, recommendedTex1Size.h);
				#endif

				openFrameBuffer();
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

// 	Sizei recommendedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
// 	Sizei recommendedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);
// 	//Sizei renderTargetSize;
// 	renderTargetSize.w = recommendedTex0Size.w + recommendedTex1Size.w;
//
// 	#ifdef WIN32
// 	renderTargetSize.h = max(recommendedTex0Size.h, recommendedTex1Size.h);
// 	#else
// 	renderTargetSize.h = std::max(recommendedTex0Size.h, recommendedTex1Size.h);
// 	#endif

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

	SDL_SysWMinfo info;

	SDL_VERSION(&info.version);

	SDL_GetWindowWMInfo(window, &info);

	ovrGLConfig cfg;
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	cfg.OGL.Header.BackBufferSize = Sizei(hmd->Resolution.w, hmd->Resolution.h);
	cfg.OGL.Header.Multisample = 1;
#if defined(OVR_OS_WIN32)
	if (!(hmd->HmdCaps & ovrHmdCap_ExtendDesktop))
		ovrHmd_AttachToWindow(hmd, info.info.win.window, NULL, NULL);

	cfg.OGL.Window = info.info.win.window;
	cfg.OGL.DC = NULL;
#elif defined(OVR_OS_LINUX)
	cfg.OGL.Disp = info.info.x11.display;
	cfg.OGL.Win = info.info.x11.window;
#endif

	ovrEyeRenderDesc eyeRenderDesc[2];

	ovrHmd_ConfigureRendering(hmd, &cfg.Config, ovrDistortionCap_Chromatic | ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive, eyeFov, eyeRenderDesc);

	ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);

}

void OvrDevice::openFrameBuffer() {

	glewExperimental = GL_TRUE;

	glewInit();

	//Cria o  Frame Buffer Object (FBO)
	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject); //Aciona o FBO

	//Cria o Render Buffer Object (RBO)
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);//Aciona o RBO
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);//aloca espaco no render buffer

	//Cria a textura no tamanho e cores definidos na da tela
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//anexa a textura para o FBO (color attachement point)
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//anexa o RBO (depth attachement point)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, DrawBuffers);

	//Verifica o status do FBO
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		closeFrameBuffer();

		//SDL_GL_DeleteContext(context);
		//SDL_DestroyWindow(window);

		//ovrHmd_Destroy(hmd);
		//ovr_Shutdown();

		//SDL_Quit();
		throw ExceptionSDL( ExceptionCode::CREATE, std::string( "Falha ao instanciar o framebuffer"));

	}
}

void OvrDevice::closeFrameBuffer() {

	glDeleteFramebuffers(1, &frameBufferObject);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderBuffer);

}

void OvrDevice::initGL (){}
void OvrDevice::initDraw(){}
void OvrDevice::endDraw(){}
void OvrDevice::getGeometry(int &_x, int &_y, int &_w, int &_h){}

}