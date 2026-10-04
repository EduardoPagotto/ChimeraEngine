#pragma once
#include "chimera_base/ICanva.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <string>

// #define GLEW_STATIC

namespace ce {

    class CanvasGL final : public ICanva {
      public:
        explicit CanvasGL(const std::string& title, int width, int height, bool full_screen = false);
        virtual ~CanvasGL();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int width, int height) override;
        virtual uint32_t getWidth() const override { return width_; }
        virtual uint32_t getHeight() const override { return height_; }

      private:
        std::string title_;
        int width_;
        int height_;
        bool full_screen_{false};
        glm::ivec2 position_;
        SDL_Window* window_{nullptr};
        SDL_GLContext context_;
    };
} // namespace ce
