#include "chimera/core/StateStack.hpp"
#include <algorithm>

namespace Chimera {

StateStack::StateStack() { layerInsert = layers.begin(); }

StateStack::~StateStack() {
    for (IStateMachine* layer : layers) {
        delete layer;
    }
}

void StateStack::pushLayer(IStateMachine* layer) { layerInsert = layers.emplace(layerInsert, layer); }

void StateStack::pushOverlay(IStateMachine* overlay) { layers.emplace_back(overlay); }

void StateStack::popLayer(IStateMachine* layer) {

    auto it = std::find(layers.begin(), layers.end(), layer);
    if (it != layers.end()) {

        layers.erase(it);
        layerInsert--;
    }
}

void StateStack::popOverlay(IStateMachine* overlay) {

    auto it = std::find(layers.begin(), layers.end(), overlay);
    if (it != layers.end())
        layers.erase(it);
}

} // namespace Chimera