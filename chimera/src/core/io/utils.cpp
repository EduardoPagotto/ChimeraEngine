#include "chimera/core/io/utils.hpp"
#include "chimera/core/Exception.hpp"
#include <fstream>

namespace Chimera::IO {

void utilSendEvent(const int32_t& user_event, void* _paramA, void* _paramB) {
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_USEREVENT;
    event.user.code = user_event;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent(&event);
}

std::string utilReadFile(const char* file_path) {

    // Read file to string
    std::string ret;
    std::ifstream file(file_path, std::ios::in);
    if (file.is_open()) {
        std::string Line = "";
        while (getline(file, Line)) {
            ret += "\n" + Line;
        }
        file.close();
    } else {
        throw Chimera::Exception("Impossivel abrir arquivo: " + std::string(file_path));
    }

    return ret;
}

} // namespace Chimera::IO