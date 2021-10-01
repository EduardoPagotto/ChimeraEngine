#include "chimera/render/2d/layer/LayerStack.hpp"

namespace Chimera {

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {}

void LayerStack::pushLayer(ILayer* layer) {}

void LayerStack::pushOverlay(ILayer* overlay) {}

void LayerStack::popLayer(ILayer* layer) {}

void LayerStack::popOverlay(ILayer* layer) {}

} // namespace Chimera
