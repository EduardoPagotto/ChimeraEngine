#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "TileLayer.hpp"
#include "chimera/core/ShaderLibrary.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/io/LayerStack.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/2d/layer/Layer.hpp"
#include "chimera/render/FontAtlas.hpp"

class Game : public Chimera::IEvents {
  public:
    Game(Chimera::CanvasGL* _pVideo);
    virtual ~Game();
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::CanvasGL* pVideo;
    Chimera::ShaderLibrary sl;
    Chimera::Shader* shader;
    TileLayer *layer, *layer2;
    Chimera::Label* lFPS;
    uint16_t x, y;

    int fps;
    Chimera::FontAtlas* fa;
    Chimera::LayerStack* layerStack;
};

#endif