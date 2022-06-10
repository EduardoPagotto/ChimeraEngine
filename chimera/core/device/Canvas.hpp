#pragma once
#include "chimera/core/TagComponent.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

/**
 * Class Canvas
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @update 20220610
 */
class Canvas {
  public:
    Canvas(const std::string& _title, int _width, int _height, bool _fullScreen = false)
        : title(_title), width(_width), height(_height), fullScreen(_fullScreen), window(nullptr) {}

    virtual ~Canvas() {}
    virtual void before() = 0;
    virtual void after() = 0;
    virtual void toggleFullScreen() = 0;
    virtual void reshape(int _width, int _height) = 0;
    virtual uint32_t* getPixels() = 0;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

  protected:
    bool fullScreen;
    int width, height;
    int posX, posY;
    std::string title;
    SDL_Window* window;
};

struct CanvasComponent {
    Canvas* canvas = nullptr;
    CanvasComponent() = default;
};

} // namespace Chimera
