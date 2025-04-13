#include "chimera/core/utils.hpp"
#include <fstream>
#include <sstream>

namespace ce {

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

void utilsReadFile(const std::string& filepath, std::string& result) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File \"%s\" not found.", filepath.c_str());
        throw std::string("File not found: " + filepath);
    }
}

std::string extractNameByFile(const std::string& filepath) {
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    return filepath.substr(lastSlash, count);
}

void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter) {
    std::string token;
    std::istringstream tokenStream(sIn);
    while (std::getline(tokenStream, token, delimiter))
        vOut.push_back(token);
}

void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData) {
        if (val.size() != 0)
            arrayFloat.push_back(std::stod(val));
    }
}

void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI) {
    std::vector<std::string> textData;
    textToStringArray(text, textData, ' ');
    for (const std::string& val : textData)
        arrayI.push_back(static_cast<uint32_t>(std::stoul(val)));
}
} // namespace ce