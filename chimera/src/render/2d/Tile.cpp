#include "chimera/render/2d/Tile.hpp"
namespace Chimera {
Tile::Tile(const std::string& name, IRenderer2D* renderer, Shader shader, Camera* camera) : Layer(renderer, shader, camera, name) {}
Tile::~Tile() {}
void Tile::onAttach(){};
void Tile::onDeatach(){};
void Tile::onUpdate(ViewProjection& vp, const double& ts){};
bool Tile::onEvent(const SDL_Event& event) { return true; }
void Tile::onRender() { Layer::onRender(); }
} // namespace Chimera