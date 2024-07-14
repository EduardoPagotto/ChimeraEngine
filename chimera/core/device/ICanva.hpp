#pragma once
#include "chimera/core/TagComponent.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

/**
 * Class ICanva
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 *  @update 20220610
 */
class ICanva {
  public:
    virtual ~ICanva() = default;
    virtual void before() = 0;
    virtual void after() = 0;
    virtual void toggleFullScreen() = 0;
    virtual void reshape(int _width, int _height) = 0;
    virtual uint32_t* getPixels() = 0;
    virtual const int getWidth() const = 0;
    virtual const int getHeight() const = 0;
};

struct CanvasComponent {
    ICanva* canvas = nullptr;
    CanvasComponent() = default;
};

} // namespace Chimera
