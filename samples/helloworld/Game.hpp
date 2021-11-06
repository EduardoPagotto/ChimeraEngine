#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "TileLayer.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/ApplicationGL.hpp"

class Game : public Chimera::ApplicationGL {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Shader shader;
    Chimera::Label* lFPS;
    TileLayer* layer;
    int fps;
};

#endif