#include "chimera/core/Canvas.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

Canvas::Canvas(const std::string& _title, int _width, int _height, bool _fullScreen)
    : title(_title), width(_width), height(_height), fullScreen(_fullScreen) {

    window = nullptr;
    // Inicializa o SDL se nao estiver inicializado
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw Exception(std::string(std::string("Falha SDL_Init:") + SDL_GetError()));
    }
}

Canvas::~Canvas() { SDL_Quit(); }

} // namespace Chimera
