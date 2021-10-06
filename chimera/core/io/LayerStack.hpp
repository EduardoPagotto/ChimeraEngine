#ifndef __CHIMERA_LAYER_STACK__HPP
#define __CHIMERA_LAYER_STACK__HPP

#include "Ilayer.hpp"
#include <vector>

namespace Chimera::Core {

class LayerStack {
  public:
    LayerStack();
    ~LayerStack();

    void pushLayer(ILayer* layer);
    void pushOverlay(ILayer* overlay);
    void popLayer(ILayer* layer);
    void popOverlay(ILayer* overlay);

    std::vector<ILayer*>::iterator begin() { return layers.begin(); }
    std::vector<ILayer*>::iterator end() { return layers.end(); }

  private:
    std::vector<ILayer*> layers;
    std::vector<ILayer*>::iterator layerInsert;
};
} // namespace Chimera::Core
#endif
