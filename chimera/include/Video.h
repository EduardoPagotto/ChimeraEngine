#ifndef __CHIMERA_VIDEO__HPP
#define __CHIMERA_VIDEO__HPP

#define GLEW_STATIC

#include <string>

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

#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

namespace Chimera {

enum class KIND_DEVICE { SCREEN = 0, OVR_OCULUS, HMD_Z1 };

/**
 * Class Video
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Video {
  public:
    Video(std::string _nome, KIND_DEVICE _kindDevice);
    Video(std::string _nome, KIND_DEVICE _kindDevice, int _w, int _h);
    virtual ~Video();

    virtual void initDraw() = 0;
    virtual void endDraw() = 0;
    virtual glm::mat4
    getPerspectiveProjectionMatrix(const float& _fov, const float& _near,
                                   const float& _far,
                                   int _eye) = 0; //(Camera *pCamera, int _eye) = 0;
    virtual glm::mat4 getOrthoProjectionMatrix(
        int eyeIndex) = 0; // virtual void executeViewOrto ( int eyeIndex ) = 0;
    virtual void reshape(int _w, int _y) = 0;
    virtual void toggleFullScreen() = 0;

    void initGL();

    int getWinSizeW() const { return winSizeW; }

    int getWinSizeH() const { return winSizeH; }

    std::string getNomeTela() const { return nomeTela; }

    void restoreMatrix();

    std::string getVersaoOpenGL();

    inline KIND_DEVICE getKindDevice() const { return kindDevice; }

  protected:
    void initSDL();

    int winSizeW;
    int winSizeH;

    std::string nomeTela;
    KIND_DEVICE kindDevice;

    SDL_Window* window;
    SDL_GLContext context;

    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera
#endif
