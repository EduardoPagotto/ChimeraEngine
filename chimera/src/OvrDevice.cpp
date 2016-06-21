#include "OvrDevice.h"

#ifdef OVR_SET_TO_USE

#include "ExceptionSDL.h"
#include <SDL_opengl.h>
#include <SDL_syswm.h>

namespace Chimera {

	OvrDevice::OvrDevice(std::string nomeTela) : Video(nomeTela, KIND_DEVICE::OVR_OCULUS) {

		fullscreenStatus = false;

		//inicializa o OVR
		ovr_Initialize();

		initSDL();

		//garante buffers zerados
		fbo = 0;
		fb_tex = 0;
		fb_depth = 0;

		initDevice();

	}

	void OvrDevice::initDevice() {

		bool debug = false;
		if ((hmd = ovrHmd_Create(0)) == nullptr){
			fprintf(stderr, "failed to open Oculus HMD, falling back to virtual debug HMD\n");
			if ((hmd = ovrHmd_CreateDebug(ovrHmd_DK2)) != nullptr)
				debug = true;
			else
				throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha Criar virtual debug HMD"));
		}
		printf("Inicializado HMD: %s - %s\n", hmd->Manufacturer, hmd->ProductName);

		//Ajusta window para bater com a mesma resolucao doHMD
		SDL_SetWindowSize(window, hmd->Resolution.w, hmd->Resolution.h);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		winSizeW = hmd->Resolution.w;
		winSizeH = hmd->Resolution.h;

		ovrSizei eyeres[2];
		/* enable position and rotation tracking */
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
		/* retrieve the optimal render target resolution for each eye */
		eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
		eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);


		fbSize.w = eyeres[0].w + eyeres[1].w;
		fbSize.h = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;
		update_rtarg(fbSize.w, fbSize.h);

		/* fill in the ovrGLTexture structures that describe our render target texture */
		for (int i = 0; i < 2; i++) {
			fb_ovr_tex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
			fb_ovr_tex[i].OGL.Header.TextureSize.w = fbTexSize.w;
			fb_ovr_tex[i].OGL.Header.TextureSize.h = fbTexSize.h;
			/* this next field is the only one that differs between the two eyes */
			fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.x = i == 0 ? 0 : fbSize.w / 2.0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.y = 0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Size.w = fbSize.w / 2.0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Size.h = fbSize.h;
			fb_ovr_tex[i].OGL.TexId = fb_tex;	/* both eyes will use the same texture id */

		}

		/* fill in the ovrGLConfig structure needed by the SDK to draw our stereo pair
		* to the actual HMD display (SDK-distortion mode)
		*/
		memset(&glcfg, 0, sizeof glcfg);
		glcfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		glcfg.OGL.Header.BackBufferSize.w = winSizeW;
		glcfg.OGL.Header.BackBufferSize.h = winSizeH;
		glcfg.OGL.Header.Multisample = 1;

#ifdef OVR_OS_WIN32
		glcfg.OGL.Window = GetActiveWindow();
		glcfg.OGL.DC = wglGetCurrentDC();
#elif defined(OVR_OS_LINUX)
		glcfg.OGL.Disp = glXGetCurrentDisplay();
#endif

		if (hmd->HmdCaps & ovrHmdCap_ExtendDesktop) {
			printf("running in \"extended desktop\" mode\n");
		}
		else {
			/* to sucessfully draw to the HMD display in "direct-hmd" mode, we have to
			* call ovrHmd_AttachToWindow
			* XXX: this doesn't work properly yet due to bugs in the oculus 0.4.1 sdk/driver
			*/
#ifdef WIN32
			ovrHmd_AttachToWindow(hmd, glcfg.OGL.Window, 0, 0);
#elif defined(OVR_OS_LINUX)
			ovrHmd_AttachToWindow(hmd, (void*)glXGetCurrentDrawable(), 0, 0);
#endif
			printf("running in \"direct-hmd\" mode\n");

		}

		/* enable low-persistence display and dynamic prediction for lattency compensation */
		hmd_caps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
		ovrHmd_SetEnabledCaps(hmd, hmd_caps);

		/* configure SDK-rendering and enable chromatic abberation correction, vignetting, and
		* timewrap, which shifts the image before drawing to counter any lattency between the call
		* to ovrHmd_GetEyePose and ovrHmd_EndFrame.
		*/
		distort_caps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp |
			ovrDistortionCap_Overdrive;
		if (!ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc)) {
			fprintf(stderr, "failed to configure distortion renderer\n");

		}

		/* disable the retarded "health and safety warning" */
		//ovrhmd_EnableHSWDisplaySDKRender(hmd, 0);

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		//glEnable(GL_LIGHT1);
		//glEnable(GL_NORMALIZE);

		//glClearColor(0.1, 0.1, 0.1, 1);

	}

	/* update_rtarg creates (and/or resizes) the render target used to draw the two stero views */
	void OvrDevice::update_rtarg(int width, int height)
	{
		if (!fbo) {
			/* if fbo does not exist, then nothing does... create every opengl object */
			glGenFramebuffers(1, &fbo);
			glGenTextures(1, &fb_tex);
			glGenRenderbuffers(1, &fb_depth);

			glBindTexture(GL_TEXTURE_2D, fb_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		/* calculate the next power of two in both dimensions and use that as a texture size */
		fbTexSize.w = next_pow2(width);
		fbTexSize.h = next_pow2(height);

		/* create and attach the texture that will be used as a color buffer */
		glBindTexture(GL_TEXTURE_2D, fb_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbTexSize.w, fbTexSize.h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

		/* create and attach the renderbuffer that will serve as our z-buffer */
		glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbTexSize.w, fbTexSize.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha:Frame buffer incompleto"));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("created render target: %dx%d (texture size: %dx%d)\n", width, height, fbTexSize.w, fbTexSize.h);

	}

	unsigned int OvrDevice::next_pow2(unsigned int x)
	{
		x -= 1;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return x + 1;
	}

	OvrDevice::~OvrDevice(){

		//close frame buffer, render buffer, texture
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &fb_tex);
		glDeleteRenderbuffers(1, &fb_depth);

		ovr_Shutdown();

	}

	void OvrDevice::reshape(int _w, int _h)
	{
		winSizeW = _w;
		winSizeH = _h;
	}

	void OvrDevice::toggleFullScreen(void)
	{

		if (fullscreenStatus == false) {
			/* going fullscreen on the rift. save current window position, and move it
			* to the rift's part of the desktop before going fullscreen
			*/
			SDL_GetWindowPosition(window, &winPosPrev.x, &winPosPrev.y);
			SDL_SetWindowPosition(window, hmd->WindowsPos.x, hmd->WindowsPos.y);
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

#ifdef OVR_OS_LINUX
			/* on linux for now we have to deal with screen rotation during rendering. The docs are promoting
			* not rotating the DK2 screen globally
			*/
			glcfg.OGL.Header.BackBufferSize.w = hmd->Resolution.h;
			glcfg.OGL.Header.BackBufferSize.h = hmd->Resolution.w;

			distort_caps |= ovrDistortionCap_LinuxDevFullscreen;
			ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc);
#endif

		}
		else {
			/* return to windowed mode and move the window back to its original position */
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowPosition(window, winPosPrev.x, winPosPrev.y);

#ifdef OVR_OS_LINUX
			glcfg.OGL.Header.BackBufferSize = hmd->Resolution;

			distort_caps &= ~ovrDistortionCap_LinuxDevFullscreen;
			ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc);
#endif
		}

		fullscreenStatus = !fullscreenStatus;
	}

	void OvrDevice::initDraw(){

		ovrFrameTiming frameTiming = ovrHmd_BeginFrame(hmd, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OvrDevice::endDraw(){

		/* after drawing both eyes into the texture render target, revert to drawing directly to the
		* display, and we call ovrHmd_EndFrame, to let the Oculus SDK draw both images properly
		* compensated for lens distortion and chromatic abberation onto the HMD screen.
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);

		/* workaround for the oculus sdk distortion renderer bug, which uses a shader
		* program, and doesn't restore the original binding when it's done.
		*/
		glUseProgram(0);

		//assert(glGetError() == GL_NO_ERROR);//FIXME

	}

	//void OvrDevice::executeViewOrto(int eyeIndex) {

	//	float orthomatrix[4][4];

	//	float right = winSizeW /2 ;
	//	float left = 0;		  //trocado;
	//	float top = winSizeH; //0;
	//	float bottom = 0;
	//	float Zfar = -1;
	//	float Znear = 1;

	//	float k1 = 2.0;
	//	float k2 = 2.0;
	//	float k3 = 2.0;

	//	orthomatrix[0][0] = k1 / (right - left);
	//	orthomatrix[0][1] = 0;
	//	orthomatrix[0][2] = 0;
	//	orthomatrix[0][3] = 0;

	//	orthomatrix[1][0] = 0;
	//	orthomatrix[1][1] = k2 / (top - bottom);
	//	orthomatrix[1][2] = 0;
	//	orthomatrix[1][3] = 0;//

	//	orthomatrix[2][0] = 0;
	//	orthomatrix[2][1] = 0;
	//	orthomatrix[2][2] = k3 / (Zfar - Znear);
	//	orthomatrix[2][3] = 0;// -(Zfar + Znear) / (Zfar - Znear);

	//	orthomatrix[3][0] = -(right + left) / (right - left);
	//	orthomatrix[3][1] = -(top + bottom) / (top - bottom);
	//	orthomatrix[3][2] = -(Zfar + Znear) / (Zfar - Znear);
	//	orthomatrix[3][3] = 1;


	//	ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
	//	pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);

	//	//begin orto
	//	glMatrixMode(GL_PROJECTION);
	//	glPushMatrix();
	//	glLoadIdentity();

	//	glLoadMatrixf(orthomatrix[0]);

	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glLoadIdentity();
	//}


	//void OvrDevice::executeViewOrto(int eyeIndex) {

	//	ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
	//	pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);

	//	//begin orto
	//	glMatrixMode(GL_PROJECTION);
	//	glPushMatrix();
	//	glLoadIdentity();

	//	glLoadTransposeMatrixf(orthoProjection[eye].M[0]);

	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glLoadIdentity();
	//}


	void OvrDevice::executeViewOrto(int eyeIndex) {

		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
		//pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		//begin orto
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.x,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.w,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.y,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.h,
			-1,
			1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}


	//	https://svn.scolring.org/trunk/dependencies/oculus/Samples/OculusWorldDemo/OculusWorldDemo.cpp


	// 	// *** 2D Text - Configure Orthographic rendering.
	//
	// 	// Render UI in 2D orthographic coordinate system that maps [-1,1] range
	// 	// to a readable FOV area centered at your eye and properly adjusted.
	// 	pRender->ApplyStereoParams(renderViewport, OrthoProjection[eye]);
	// 	pRender->SetDepthMode(false, false);
	//
	// 	// We set this scale up in CreateOrthoSubProjection().
	// 	float textHeight = 22.0f;
	//
	// 	// Display Loading screen-shot in frame 0.
	// 	if (LoadingState != LoadingState_Finished)
	// 	{
	// 		const float scale = textHeight * 25.0f;
	// 		Matrix4f view ( scale, 0.0f, 0.0f, 0.0f, scale, 0.0f, 0.0f, 0.0f, scale );
	// 		LoadingScene.Render(pRender, view);
	// 		String loadMessage = String("Loading ") + MainFilePath;
	// 		DrawTextBox(pRender, 0.0f, -textHeight, textHeight, loadMessage.ToCStr(), DrawText_HCenter);
	// 		LoadingState = LoadingState_DoLoad;
	// 	}
	//
	// 	// HUD overlay brought up by spacebar.
	// 	RenderTextInfoHud(textHeight);
	//
	// 	// Menu brought up by
	// 	Menu.Render(pRender);


	// Calculate projections
	// 	Projection[0] = ovrMatrix4f_Projection(EyeRenderDesc[0].Fov,  0.01f, 10000.0f, true);
	// 	Projection[1] = ovrMatrix4f_Projection(EyeRenderDesc[1].Fov,  0.01f, 10000.0f, true);
	//
	// 	float    orthoDistance = 0.8f; // 2D is 0.8 meter from camera
	// 	Vector2f orthoScale0   = Vector2f(1.0f) / Vector2f(EyeRenderDesc[0].PixelsPerTanAngleAtCenter);
	// 	Vector2f orthoScale1   = Vector2f(1.0f) / Vector2f(EyeRenderDesc[1].PixelsPerTanAngleAtCenter);
	//
	// 	OrthoProjection[0] = ovrMatrix4f_OrthoSubProjection(Projection[0], orthoScale0, orthoDistance,
	// 														EyeRenderDesc[0].HmdToEyeViewOffset.x);
	// 	OrthoProjection[1] = ovrMatrix4f_OrthoSubProjection(Projection[1], orthoScale1, orthoDistance,
	// 														EyeRenderDesc[1].HmdToEyeViewOffset.x);




	//	OVR::Matrix4f OvrDevice::getOrthographic() {
	// 		const ovrEyeRenderDesc & erd = getEyeRenderDesc();
	// 		ovrMatrix4f ovrPerspectiveProjection = ovrMatrix4f_Projection(erd.Fov, 0.01f, 100000.0f, true);
	// 		ovrVector2f scale; scale.x = scaleFactor; scale.y = scaleFactor;
	// 		return ovrMatrix4f_OrthoSubProjection(ovrPerspectiveProjection, scale, 100.8f, erd.HmdToEyeViewOffset.x);
	// 	}
	//
	//
	// 	void OvrDevice::GetOrthoProjection(const HMDRenderState& RenderState, OVR::Matrix4f OrthoProjection[2])
	// 	{
	// 		OVR::Matrix4f perspectiveProjection[2];
	// 		perspectiveProjection[0] = ovrMatrix4f_Projection(RenderState.EyeRenderDesc[0].Fov, 0.01f, 10000.f, true);
	// 		perspectiveProjection[1] = ovrMatrix4f_Projection(RenderState.EyeRenderDesc[1].Fov, 0.01f, 10000.f, true);
	//
	// 		const float    orthoDistance = 10;//HSWDISPLAY_DISTANCE; // This is meters from the camera (viewer) that we place the ortho plane.
	// 		const Vector2f orthoScale0   = OVR::Vector2f(1.f) / OVR::Vector2f(RenderState.EyeRenderDesc[0].PixelsPerTanAngleAtCenter);
	// 		const Vector2f orthoScale1   = OVR::Vector2f(1.f) / OVR::Vector2f(RenderState.EyeRenderDesc[1].PixelsPerTanAngleAtCenter);
	//
	// 		OrthoProjection[0] = ovrMatrix4f_OrthoSubProjection(perspectiveProjection[0], orthoScale0, orthoDistance, RenderState.EyeRenderDesc[0].ViewAdjust.x);
	// 		OrthoProjection[1] = ovrMatrix4f_OrthoSubProjection(perspectiveProjection[1], orthoScale1, orthoDistance, RenderState.EyeRenderDesc[1].ViewAdjust.x);
	// 	}

	void OvrDevice::executeViewPerspective(const float &_fov,const float &_near,const float &_far, int _eye) {

		using namespace OVR;

		const float orthoDistance = 10;//HSWDISPLAY_DISTANCE
		//ovrMatrix4f orthoProjection;
		ovrMatrix4f perspectiveProjection;
		float rot_mat[16];

		ovrEyeType eye = hmd->EyeRenderOrder[_eye];
		glViewport(eye == ovrEye_Left ? 0 : fbSize.w / 2, 0, fbSize.w / 2, fbSize.h);


		const Vector2f orthoScale = Vector2f(1.f) / Vector2f(eye_rdesc[eye].PixelsPerTanAngleAtCenter);

		//perspectiveProjection = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
		perspectiveProjection = ovrMatrix4f_Projection(eye_rdesc[eye].Fov, 0.1, 1000.0, true);

		orthoProjection[_eye] = ovrMatrix4f_OrthoSubProjection(perspectiveProjection, orthoScale, orthoDistance, eye_rdesc[eye].DistortedViewport.Pos.x);

		//orthoProjection[eye].M[1][1] = -orthoProjection[eye].M[1][1];
		//orthoProjection[eye].M[1][1] = -orthoProjection[eye].M[1][1];

		glMatrixMode(GL_PROJECTION);
		glLoadTransposeMatrixf(perspectiveProjection.M[0]);

		/* -- view/camera transformation --
		* we need to construct a view matrix by combining all the information provided by the oculus
		* SDK, about the position and orientation of the user's head in the world.
		*/
		/* TODO: use ovrHmd_GetEyePoses out of the loop instead */
		pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(eye_rdesc[eye].HmdToEyeViewOffset.x,
			eye_rdesc[eye].HmdToEyeViewOffset.y,
			eye_rdesc[eye].HmdToEyeViewOffset.z);
		/* retrieve the orientation quaternion and convert it to a rotation matrix */
		quat_to_matrix(&pose[eye].Orientation.x, rot_mat);
		glMultMatrixf(rot_mat);
		/* translate the view matrix with the positional tracking */
		glTranslatef(-pose[eye].Position.x, -pose[eye].Position.y, -pose[eye].Position.z);
		/* move the camera to the eye level of the user */
		glTranslatef(0, -ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.65), 0);

	}

	/* convert a quaternion to a rotation matrix */
	void OvrDevice::quat_to_matrix(const float *quat, float *mat)
	{
		mat[0] = 1.0 - 2.0 * quat[1] * quat[1] - 2.0 * quat[2] * quat[2];
		mat[4] = 2.0 * quat[0] * quat[1] + 2.0 * quat[3] * quat[2];
		mat[8] = 2.0 * quat[2] * quat[0] - 2.0 * quat[3] * quat[1];
		mat[12] = 0.0f;

		mat[1] = 2.0 * quat[0] * quat[1] - 2.0 * quat[3] * quat[2];
		mat[5] = 1.0 - 2.0 * quat[0] * quat[0] - 2.0 * quat[2] * quat[2];
		mat[9] = 2.0 * quat[1] * quat[2] + 2.0 * quat[3] * quat[0];
		mat[13] = 0.0f;

		mat[2] = 2.0 * quat[2] * quat[0] + 2.0 * quat[3] * quat[1];
		mat[6] = 2.0 * quat[1] * quat[2] - 2.0 * quat[3] * quat[0];
		mat[10] = 1.0 - 2.0 * quat[0] * quat[0] - 2.0 * quat[1] * quat[1];
		mat[14] = 0.0f;

		mat[3] = mat[7] = mat[11] = 0.0f;
		mat[15] = 1.0f;
	}

}

#endif //  OVR_SET_TO_USE