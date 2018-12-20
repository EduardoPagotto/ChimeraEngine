#ifndef __CHIMERA_CANVAS__HPP
#define __CHIMERA_CANVAS__HPP

#define GLEW_STATIC

#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

// enum class KIND_DEVICE { SCREEN = 0, OVR_OCULUS, HMD_Z1 };

/**
 * Class Canvas
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @update 20181206
 */
class Canvas {
  public:
    Canvas(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~Canvas();

    virtual void before() = 0;
    virtual void after() = 0;
    virtual void toggleFullScreen() = 0;
    virtual void reshape(int _width, int _height) = 0;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline bool getFullScreen() const { return fullScreen; }

  protected:
    bool fullScreen;
    int width;
    int height;
    std::string title;
    SDL_Window* window;
    SDL_Point winPosPrev;
};
} // namespace Chimera
#endif
