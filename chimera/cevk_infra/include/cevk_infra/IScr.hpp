#pragma once
#include <SDL3/SDL.h>

namespace ce {

    /// @brief Canva Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270731
    class IScr {

      public:
        virtual ~IScr() = default;
        virtual void before() = 0;
        virtual void after() = 0;
        virtual void toggleFullScreen() = 0;
        virtual void reshape(int _width, int _height) = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        // FB only
        // virtual uint32_t* getPixels() = 0;
        // virtual SDL_PixelFormat getPixelFormat() = 0;
    };
} // namespace ce
