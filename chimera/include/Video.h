#ifndef VIDEO_H_
#define VIDEO_H_

//TODO verificar em Linux se sera necessario
//#include <cstddef>
//#include <string>
//#include <iostream>

// #ifdef WIN32
// #include "windows.h"
// #endif
//
// #include "GL/glew.h"
//
// #include <GL/gl.h>
// #include <GL/glu.h>
//
// #ifndef WIN32
// #include <SDL2/SDL.h>
// #else
// #include <SDL.h>
// #endif

#include <string>
#include "Camera.h"
#include "HUD.h"

namespace Chimera
{

  enum class KIND_DEVICE {
    SCREEN = 0,
    OVR_OCULUS
  };

/**
 * Class Video
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
  class Video
  {
  public:

    Video(std::string _nome);

    virtual ~ Video ();

    //virtual void initHardware() = 0;

    virtual void initGL () = 0;
    virtual void initDraw() = 0;
    virtual void endDraw() = 0;
    virtual void getGeometry(int &_x, int &_y, int &_w, int &_h, int index) = 0;

    virtual void executeViewPerspective(Camera *pCamera, int _eye) = 0;
	virtual void executeViewOrto(int eyeIndex) = 0;

//     const SDL_Rect *getPRectangle () const{
//       return &rectangle;
//     }

    std::string getNomeTela () const {
      return nomeTela;
    }

	void updateHud(HUD *pHud, int eye);

	void restoreMatrix();

    //void setViewPortOrtogonal (const int &_x, const int &_y, const int &_w, const int &_h);
    //void setViewPortPerspective ( const int &_x, const int &_y, const int &_w, const int &_h, const float &_fov, const float &_near, const float &_far);

    void begin2D ();
    void end2D ();

	

    void setLight (bool _lightOn);
    void setMaterial (bool _materialOn);

    std::string getVersaoOpenGL ();

    inline KIND_DEVICE getKindDevice() const {
      return kindDevice;
    }

  protected:
    std::string nomeTela;
    KIND_DEVICE kindDevice;
  //  SDL_Rect rectangle;
    //SDL_Window *window;
   // SDL_GLContext context;

   // GLuint frameBufferObject;
   // GLuint renderBuffer;
   // GLuint texture;

  };

}				/* namespace Chimera */
#endif				/* VIDEO_H_ */
// kate: indent-mode cstyle; indent-width 2; replace-tabs on; ;
