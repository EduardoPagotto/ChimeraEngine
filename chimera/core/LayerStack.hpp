#ifndef __CHIMERA_LAYER_STACK__HPP
#define __CHIMERA_LAYER_STACK__HPP

#include "ILayer.hpp"
#include <vector>

namespace Chimera {

class LayerStack {
  public:
    LayerStack();
    ~LayerStack();
    inline void clear() { layers.clear(); }

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
} // namespace Chimera
#endif