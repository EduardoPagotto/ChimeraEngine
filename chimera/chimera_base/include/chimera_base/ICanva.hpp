#pragma once
#include <stdint.h>

namespace ce {

    /// @brief Canva Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270731
    class ICanva {

      public:
        virtual ~ICanva() = default;
        virtual void before() = 0;
        virtual void after() = 0;
        virtual void toggleFullScreen() = 0;
        virtual void reshape(int _width, int _height) = 0;
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
    };
} // namespace ce
