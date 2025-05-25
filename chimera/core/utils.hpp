#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace ce {

    void utilsReadFile(const std::string& filepath, std::string& result);
    std::string extractNameByFile(const std::string& filepath);

    inline int16_t dead16(const int16_t& vIn, const int16_t& deadzone) {
        return (vIn + (vIn >> 16) ^ (vIn >> 16)) > deadzone ? vIn : 0;
    }
    inline float scale16(const int16_t& value, const int16_t& limit) {
        return value >= 0 ? (float)value / (limit - 1) : (float)value / limit;
    }

    void textToStringArray(const std::string& sIn, std::vector<std::string>& vOut, char delimiter);
    void textToFloatArray(const std::string& text, std::vector<float>& arrayFloat);
    void textToUIntArray(const std::string& text, std::vector<uint32_t>& arrayI);

} // namespace ce
