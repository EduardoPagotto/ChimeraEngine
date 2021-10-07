#include "chimera/core/io/LayerStack.hpp"
#include <algorithm>

namespace Chimera {

LayerStack::LayerStack() { layerInsert = layers.begin(); }

LayerStack::~LayerStack() {
    for (ILayer* layer : layers) {
        delete layer;
    }
}
// https://github.com/Ershany/Arcane-Engine/blob/master/Arcane/src/Arcane/Core/Application.cpp ver!!!!!
void LayerStack::pushLayer(ILayer* layer) { layerInsert = layers.emplace(layerInsert, layer); }

void LayerStack::pushOverlay(ILayer* overlay) { layers.emplace_back(overlay); }

void LayerStack::popLayer(ILayer* layer) {

    auto it = std::find(layers.begin(), layers.end(), layer);
    if (it != layers.end()) {

        layers.erase(it);
        layerInsert--;
    }
}

void LayerStack::popOverlay(ILayer* overlay) {

    auto it = std::find(layers.begin(), layers.end(), overlay);
    if (it != layers.end())
        layers.erase(it);
}

} // namespace Chimera
