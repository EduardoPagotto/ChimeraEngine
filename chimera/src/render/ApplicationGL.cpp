#include "chimera/render/ApplicationGL.hpp"

namespace Chimera {

ApplicationGL::~ApplicationGL() {
    for (ILayer* l : layerStack) { // TODO: melhorar remover apenas os existentes (fazer o deatach!!)
        l->onDeatach();
        delete l;
    }
    layerStack.clear();
}

void ApplicationGL::pushLayer(ILayer* layer) {
    layerStack.pushLayer(layer);
    layer->onAttach();
}

void ApplicationGL::pushOverlay(ILayer* overlay) {
    layerStack.pushOverlay(overlay);
    overlay->onAttach();
}

void ApplicationGL::onAttach() {}

void ApplicationGL::onDeatach() {}

void ApplicationGL::onUpdate() {
    for (auto it = layerStack.begin(); it != layerStack.end(); it++)
        (*it)->onUpdate();
}

void ApplicationGL::onRender() {
    for (auto it = layerStack.begin(); it != layerStack.end(); it++)
        (*it)->render();
}

bool ApplicationGL::onEvent(const SDL_Event& event) {

    for (auto it = layerStack.end(); it != layerStack.begin();) {
        if ((*--it)->onEvent(event) == false)
            return false;
    }

    return true;
}
} // namespace Chimera