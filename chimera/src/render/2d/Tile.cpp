#include "chimera/render/2d/Tile.hpp"

Tile::Tile(Chimera::IRenderer2D* renderer, Chimera::Shader shader, Chimera::ICamera* camera) : Chimera::Layer(renderer, shader, camera) {}
Tile::~Tile() {}
void Tile::onAttach(){};
void Tile::onDeatach(){};
void Tile::onUpdate(const double& ts){};
bool Tile::onEvent(const SDL_Event& event) {

    switch (event.type) {
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    camera->setViewportSize(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}
void Tile::onRender() { Layer::onRender(); }
