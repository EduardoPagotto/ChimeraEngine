#include "OvrDevice.h"
#include "ExceptionSDL.h"

#include <SDL_opengl.h>
#include <SDL_syswm.h>

namespace Chimera {

	OvrDevice::OvrDevice(std::string nomeTela) : Video(nomeTela) {

		//Define o device como OVR
		kindDevice = KIND_DEVICE::OVR_OCULUS;

		//inicializa o OVR
		ovr_Initialize();

		//Inicializa o SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string(std::string("Falha init SDL:") + SDL_GetError()));

		int x = SDL_WINDOWPOS_UNDEFINED;
		int y = SDL_WINDOWPOS_UNDEFINED;
		Uint32 flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_SHOWN;

		if ((window = SDL_CreateWindow(nomeTela.c_str(),	//window title
			x,				//initial x position
			y,				//initial y position
			1024,				//width, in pixels
			640,				//height, in pixels
			flags)) == nullptr) {

			throw ExceptionSDL(ExceptionCode::CREATE, std::string(std::string("Falha Criar Janela SDL:") + SDL_GetError()));
		}

		if ((context = SDL_GL_CreateContext(window)) == nullptr) {
			throw ExceptionSDL(ExceptionCode::CREATE, std::string(std::string("Falha Criar contexto SDL:") + SDL_GetError()));
		}

		glewExperimental = GL_TRUE;

		initDevice();

	}

	void OvrDevice::initDevice() {

		fbo = 0;
		fb_depth = 0;
		fb_tex = 0;

		//iniciala GLEW
		glewInit();

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

		win_width = hmd->Resolution.w;
		win_height = hmd->Resolution.h;

		ovrSizei eyeres[2];
		/* enable position and rotation tracking */
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
		/* retrieve the optimal render target resolution for each eye */
		eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
		eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);

		
		fb_width = eyeres[0].w + eyeres[1].w;
		fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;
		update_rtarg(fb_width, fb_height);

		/* fill in the ovrGLTexture structures that describe our render target texture */
		for (int i = 0; i < 2; i++) {
			fb_ovr_tex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
			fb_ovr_tex[i].OGL.Header.TextureSize.w = fb_tex_width;
			fb_ovr_tex[i].OGL.Header.TextureSize.h = fb_tex_height;
			/* this next field is the only one that differs between the two eyes */
			fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.x = i == 0 ? 0 : fb_width / 2.0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.y = 0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Size.w = fb_width / 2.0;
			fb_ovr_tex[i].OGL.Header.RenderViewport.Size.h = fb_height;
			fb_ovr_tex[i].OGL.TexId = fb_tex;	/* both eyes will use the same texture id */

		}

		/* fill in the ovrGLConfig structure needed by the SDK to draw our stereo pair
		* to the actual HMD display (SDK-distortion mode)
		*/
		memset(&glcfg, 0, sizeof glcfg);
		glcfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		glcfg.OGL.Header.BackBufferSize.w = win_width;
		glcfg.OGL.Header.BackBufferSize.h = win_height;
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
		fb_tex_width = next_pow2(width);
		fb_tex_height = next_pow2(height);

		/* create and attach the texture that will be used as a color buffer */
		glBindTexture(GL_TEXTURE_2D, fb_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb_tex_width, fb_tex_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

		/* create and attach the renderbuffer that will serve as our z-buffer */
		glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fb_tex_width, fb_tex_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha:Frame buffer incompleto"));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("created render target: %dx%d (texture size: %dx%d)\n", width, height, fb_tex_width, fb_tex_height);

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

		if (context != nullptr)
			SDL_GL_DeleteContext(context);

		if (window != nullptr)
			SDL_DestroyWindow(window);

		if (hmd != nullptr)
			ovrHmd_Destroy(hmd);

		ovr_Shutdown();

	}

	void OvrDevice::reshape(int x, int y)
	{
		win_width = x;
		win_height = y;
	}

	void OvrDevice::toggleFullScreen(void)
	{
		static int fullscr, prev_x, prev_y;
		fullscr = !fullscr;

		if (fullscr) {
			/* going fullscreen on the rift. save current window position, and move it
			* to the rift's part of the desktop before going fullscreen
			*/
			SDL_GetWindowPosition(window, &prev_x, &prev_y);
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

		} else {
			/* return to windowed mode and move the window back to its original position */
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowPosition(window, prev_x, prev_y);

#ifdef OVR_OS_LINUX
			glcfg.OGL.Header.BackBufferSize = hmd->Resolution;

			distort_caps &= ~ovrDistortionCap_LinuxDevFullscreen;
			ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc);
#endif

		}
	}

	void OvrDevice::initOVRSubSys(){

		// 		ovrFovPort eyeFov[2] = { hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1] };
		// 
		// 		eyeRenderViewport[0].Pos = OVR::Vector2i(0, 0);
		// 		eyeRenderViewport[0].Size = OVR::Sizei(renderTargetSize.w / 2, renderTargetSize.h);
		// 		eyeRenderViewport[1].Pos = OVR::Vector2i((renderTargetSize.w + 1) / 2, 0);
		// 		eyeRenderViewport[1].Size = eyeRenderViewport[0].Size;
		// 
		// 		//ovrGLTexture eyeTexture[2];
		// 		eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
		// 		eyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
		// 		eyeTexture[0].OGL.Header.RenderViewport = eyeRenderViewport[0];
		// 		eyeTexture[0].OGL.TexId = texture;
		// 
		// 		eyeTexture[1] = eyeTexture[0];
		// 		eyeTexture[1].OGL.Header.RenderViewport = eyeRenderViewport[1];
		// 
		// 		SDL_SysWMinfo info;
		// 
		// 		SDL_VERSION(&info.version);
		// 
		// 		SDL_GetWindowWMInfo(window, &info);
		// 
		// 		ovrGLConfig cfg;
		// 		cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		// 		cfg.OGL.Header.BackBufferSize = OVR::Sizei(hmd->Resolution.w, hmd->Resolution.h);
		// 		cfg.OGL.Header.Multisample = 1;
		// #if defined(OVR_OS_WIN32)
		// 		if (!(hmd->HmdCaps & ovrHmdCap_ExtendDesktop))
		// 			ovrHmd_AttachToWindow(hmd, info.info.win.window, NULL, NULL);
		// 
		// 		cfg.OGL.Window = info.info.win.window;
		// 		cfg.OGL.DC = NULL;
		// #elif defined(OVR_OS_LINUX)
		// 		cfg.OGL.Disp = info.info.x11.display;
		// 		//cfg.OGL.Win = info.info.x11.window; //FIXME verificar o que vai no lugar
		// #endif
		// 
		// 		ovrHmd_ConfigureRendering(hmd, &cfg.Config, ovrDistortionCap_Chromatic | ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive, eyeFov, eyeRenderDesc);
		// 		ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);
		// 		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
	}

	void OvrDevice::openFrameBuffer() {


		// 		//Cria o  Frame Buffer Object (FBO)
		// 		glGenFramebuffers(1, &frameBufferObject);
		// 		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject); //Aciona o FBO
		// 
		// 		//Cria o Render Buffer Object (RBO)
		// 		glGenRenderbuffers(1, &renderBuffer);
		// 		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);//Aciona o RBO
		// 		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);//aloca espaco no render buffer
		// 
		// 		//Cria a textura no tamanho e cores definidos na da tela
		// 		glGenTextures(1, &texture);
		// 		glBindTexture(GL_TEXTURE_2D, texture);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		// 
		// 		//anexa a textura para o FBO (color attachement point)
		// 		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		// 		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		// 
		// 		//anexa o RBO (depth attachement point)
		// 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		// 
		// 		//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		// 		//glDrawBuffers(1, DrawBuffers);
		// 
		// 		//Verifica o status do FBO
		// 		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		// 		{
		// 			closeFrameBuffer();
		// 
		// 			//SDL_GL_DeleteContext(context);
		// 			//SDL_DestroyWindow(window);
		// 
		// 			//ovrHmd_Destroy(hmd);
		// 			//ovr_Shutdown();
		// 
		// 			//SDL_Quit();
		// 			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha ao instanciar o framebuffer"));
		// 
		// 		}
	}

	void OvrDevice::closeFrameBuffer() {

		// 		glDeleteFramebuffers(1, &frameBufferObject);
		// 		glDeleteTextures(1, &texture);
		// 		glDeleteRenderbuffers(1, &renderBuffer);

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

		//ovrHmd_EndFrame(hmd, eyeRenderPose, &eyeTexture[0].Texture);
		ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);

		/* workaround for the oculus sdk distortion renderer bug, which uses a shader
		* program, and doesn't restore the original binding when it's done.
		*/
		glUseProgram(0);

		//assert(glGetError() == GL_NO_ERROR);

	}

	void OvrDevice::initGL(){

		GLenum error = GL_NO_ERROR;
		 
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		 
		//Check for error
		error = glGetError();
		 
		if (error != GL_NO_ERROR)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha ao Iniciar o OpenGL:" + std::string((const char *)gluErrorString(error))));
		 
		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		 
		//Check for error
		error = glGetError();
		 
		if (error != GL_NO_ERROR)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha ao Iniciar o OpenGL:" + std::string((const char *)gluErrorString(error))));
		 
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		 
		//Check for error
		error = glGetError();
		 
		if (error != GL_NO_ERROR)
			throw ExceptionSDL(ExceptionCode::CREATE, std::string("Falha ao Iniciar o OpenGL:" + std::string((const char *)gluErrorString(error))));
		 
		//TODO retirar daqui e colocar na inicializacao do sceneMng
		//estado inicial do openGL
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_LIGHTING);

	}


	void OvrDevice::getGeometry(int &_x, int &_y, int &_w, int &_h, int index){

		if ((index >= 0) && (index < 2)) {
		 
			_x = fb_ovr_tex[index].OGL.Header.RenderViewport.Pos.x;
			_y = fb_ovr_tex[index].OGL.Header.RenderViewport.Pos.y;
			_w = fb_ovr_tex[index].OGL.Header.RenderViewport.Size.w;
			_h = fb_ovr_tex[index].OGL.Header.RenderViewport.Size.h;
		 
		}
		else {
		 	throw ExceptionSDL(ExceptionCode::READ, "Indice do eye fora de faixa");
		}


		// 		if ((index >= 0) && (index < 2)) {
		// 
		// 			_x = eyeRenderViewport[index].Pos.x;
		// 			_y = eyeRenderViewport[index].Pos.y;
		// 			_w = eyeRenderViewport[index].Size.w;
		// 			_h = eyeRenderViewport[index].Size.h;
		// 
		// 		}
		// 		else {
		// 			throw ExceptionSDL(ExceptionCode::READ, "Indice do eye fora de faixa");
		// 		}

	}

	void OvrDevice::executeViewOrto(int eyeIndex) {

		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
		pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		 
		//begin orto
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		//glOrtho(eyeRenderViewport[eye].Pos.x, eyeRenderViewport[eye].Size.w, eyeRenderViewport[eye].Pos.y, eyeRenderViewport[eye].Size.h, -1, 1);
		glOrtho(fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.x,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.w,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.y,
			fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.h,
			-1, 
			1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();


		// 		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
		// 		//eyeRenderPose[eye] = ovrHmd_GetEyePose(hmd, eye);
		// 		eyeRenderPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
		// 
		// 		//begin orto
		// 		glMatrixMode(GL_PROJECTION);
		// 		glPushMatrix();
		// 		glLoadIdentity();
		// 		glOrtho(eyeRenderViewport[eye].Pos.x, eyeRenderViewport[eye].Size.w, eyeRenderViewport[eye].Pos.y, eyeRenderViewport[eye].Size.h, -1, 1);
		// 		glMatrixMode(GL_MODELVIEW);
		// 		glPushMatrix();
		// 		glLoadIdentity();
	}

	void OvrDevice::executeViewPerspective(Camera *pCamera, int eyeIndex) {

		using namespace OVR;

		ovrMatrix4f proj;
		float rot_mat[16];

		ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
		//pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);

		glViewport(eye == ovrEye_Left ? 0 : fb_width / 2, 0, fb_width / 2, fb_height);

		proj = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadTransposeMatrixf(proj.M[0]);


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

	//void OvrDevice::executeViewPerspective(Camera *pCamera, int eyeIndex) {

	//	using namespace OVR;

	//	ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
	//	pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);

	//	//glViewport(eye == ovrEye_Left ? 0 : fb_width / 2, 0, fb_width / 2, fb_height);

	//	////fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.x
	//	glViewport(fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.x,
	//		fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Pos.y,
	//		fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.w,
	//		fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.h);

	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();

	//	//gluPerspective(pCamera->getFov(), (GLfloat)(float)eyeRenderViewport[eye].Size.w / (float)eyeRenderViewport[eye].Size.h, pCamera->getNear(), pCamera->getFar());
	//	gluPerspective(pCamera->getFov(),
	//		(GLfloat)fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.w / (float)fb_ovr_tex[eyeIndex].OGL.Header.RenderViewport.Size.h,
	//		pCamera->getNear(),
	//		pCamera->getFar());

	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();
	//}

	//	//using namespace OVR;
	//	// 
	//	//ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
	//	////eyeRenderPose[eye] = ovrHmd_GetEyePose(hmd, eye);
	//	//eyeRenderPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
	//	// 
	//	////FIXME ver em camera
	//	//// Matrix4f MVPMatrix = Matrix4f(ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.01f, 10000.0f, true)) * Matrix4f::Translation(eyeRenderDesc[eye].ViewAdjust) * Matrix4f(Quatf(eyeRenderPose[eye].Orientation).Inverted());
	//	////glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, &MVPMatrix.Transposed().M[0][0]);
	//	// 
	//	//glViewport(eyeRenderViewport[eye].Pos.x, eyeRenderViewport[eye].Pos.y, eyeRenderViewport[eye].Size.w, eyeRenderViewport[eye].Size.h);
	//	// 
	//	//glMatrixMode(GL_PROJECTION);
	//	//glLoadIdentity();
	//	//gluPerspective(pCamera->getFov(), (GLfloat)(float)eyeRenderViewport[eye].Size.w / (float)eyeRenderViewport[eye].Size.h, pCamera->getNear(), pCamera->getFar());
	//	//glMatrixMode(GL_MODELVIEW);
	//	//glLoadIdentity();

	//}

}