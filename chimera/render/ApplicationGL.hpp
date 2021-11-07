#pragma once
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/2d/layer/LayerStack.hpp"

namespace Chimera {

class ApplicationGL : public IStateMachine {
  public:
    ApplicationGL(Engine* engine) : engine(engine){};
    virtual ~ApplicationGL();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onUpdate() override;
    virtual void onRender() override;
    virtual bool onEvent(const SDL_Event& event) override;

    void pushLayer(ILayer* layer);
    void pushOverlay(ILayer* overlay);

  protected:
    Engine* engine;

  private:
    LayerStack layerStack;
};
} // namespace Chimera