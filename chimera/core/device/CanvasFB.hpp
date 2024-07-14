#pragma once

#include "ICanva.hpp"

namespace Chimera {

class CanvasFB : public ICanva {
  private:
    bool fullScreen{false};
    int width, height;
    int posX, posY;
    std::string title;
    uint32_t* pixels{nullptr};
    uint32_t pixelFormat;
    SDL_Texture* texture{nullptr};
    SDL_Renderer* renderer{nullptr};
    SDL_Window* window{nullptr};

  public:
    CanvasFB(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasFB();
    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;
    virtual const int getWidth() const override { return width; }
    virtual const int getHeight() const override { return height; }
    uint32_t* getPixels() override { return pixels; }
    uint32_t getPixelFormat() { return pixelFormat; }
};
} // namespace Chimera
