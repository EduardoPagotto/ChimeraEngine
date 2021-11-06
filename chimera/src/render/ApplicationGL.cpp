#include "chimera/render/ApplicationGL.hpp"

namespace Chimera {

ApplicationGL::ApplicationGL(Canvas* canvas) : Application(canvas) {}

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

void ApplicationGL::onStart() {}

void ApplicationGL::onUpdate() {

    for (auto it = layerStack.begin(); it != layerStack.end(); it++) {
        (*it)->onUpdate();
        (*it)->render();
    }
}

bool ApplicationGL::onEvent(const SDL_Event& event) {

    for (auto it = layerStack.end(); it != layerStack.begin();) {
        if ((*--it)->onEvent(event) == false)
            return false;
    }

    return true;
}
} // namespace Chimera