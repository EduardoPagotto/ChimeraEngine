#include "chimera/core/Video.hpp"

#include "chimera/core/Exception.hpp"

namespace Chimera {

Canvas::Canvas(const std::string& _title, int _width, int _height, bool _fullScreen)
    : title(_title), width(_width), height(_height), fullScreen(_fullScreen) {

    window = nullptr;

    // Inicializa o SDL se nao estiver inicializado
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw Exception(std::string(std::string("Falha SDL_Init:") + SDL_GetError()));
    }

    log = spdlog::get("chimera");
    log->debug("Canvas:{0} w:{1:03d} h:{2:03d}", _title, _width, _height);
}

Canvas::~Canvas() {
    log->debug("Destructor Canvas");
    SDL_Quit();
}

} // namespace Chimera
