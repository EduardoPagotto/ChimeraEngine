#ifndef __CHIMERA_CANVAS__HPP
#define __CHIMERA_CANVAS__HPP

#define GLEW_STATIC

#include "chimera/core/ViewPoint.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

/**
 * Class Canvas
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @update 20210910
 */
class Canvas {
  public:
    Canvas(const std::string& _title, int _width, int _height, bool _fullScreen = false)
        : title(_title), width(_width), height(_height), fullScreen(_fullScreen), window(nullptr) {}

    virtual ~Canvas() {}
    virtual void before(const unsigned short& _indexEye = 0) = 0;
    virtual void after(const unsigned short& _indexEye = 0) = 0;
    virtual void toggleFullScreen() = 0;
    virtual void reshape(int _width, int _height) = 0;

    virtual void swapWindow() = 0;
    virtual void calcPerspectiveProjectionView(const unsigned short& _indexEye, ViewPoint* vp, glm::mat4& pView,
                                               glm::mat4& pProjection) = 0;
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex) = 0;
    virtual int getTotEyes() = 0;

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
