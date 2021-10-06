#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "TileLayer.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/io/LayerStack.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/2d/layer/Layer.hpp"
#include "chimera/render/FontAtlas.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasGL* _pVideo);
    virtual ~Game();
    void start();
    virtual void update();
    virtual void keboardEvent(SDL_Event* pEventSDL);
    virtual void mouseEvent(SDL_Event* pEventSDL);
    virtual void joystickEvent(SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::Core::CanvasGL* pVideo;
    Chimera::ShaderLibrary sl;
    Chimera::Shader* shader;
    TileLayer *layer, *layer2;
    Chimera::Label* lFPS;
    uint16_t x, y;

    int fps;
    Chimera::FontAtlas* fa;
    Chimera::Core::LayerStack* layerStack;
};

#endif