#include "chimera/render/2d/Tile.hpp"

Tile::Tile(const std::string& name, Chimera::IRenderer2D* renderer, Chimera::Shader shader, Chimera::Camera* camera)
    : nameTile(name), Chimera::Layer(renderer, shader, camera) {}
Tile::~Tile() {}
void Tile::onAttach(){};
void Tile::onDeatach(){};
void Tile::onUpdate(const double& ts){};
bool Tile::onEvent(const SDL_Event& event) { return true; }
void Tile::onRender() { Layer::onRender(); }
