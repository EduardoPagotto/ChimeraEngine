#pragma once
#include "chimera/core/Application.hpp"
#include "chimera/render/2d/layer/LayerStack.hpp"

namespace Chimera {

class ApplicationGL : public Application {
  public:
    ApplicationGL(Canvas* canvas);
    virtual ~ApplicationGL();
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;
    void pushLayer(ILayer* layer);
    void pushOverlay(ILayer* overlay);

  protected:
  private:
    LayerStack layerStack;
};
} // namespace Chimera