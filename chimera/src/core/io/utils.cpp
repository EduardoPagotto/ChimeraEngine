#include "chimera/core/io/utils.hpp"
#include "chimera/core/Exception.hpp"
#include <fstream>

namespace Chimera::Core {

void utilSendEvent(const int32_t& user_event, void* _paramA, void* _paramB) {
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_USEREVENT;
    event.user.code = user_event;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    if (SDL_PushEvent(&event) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical PushEvent fail: %s", SDL_GetError());
    }
}

std::string utilReadFile(const std::string& filepath) {

    std::string result;
    std::ifstream in(filepath, std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File \"%s\" not found.", filepath.c_str());
        throw Chimera::Exception("File not found: " + std::string(filepath));
    }

    return result;
}

} // namespace Chimera::Core