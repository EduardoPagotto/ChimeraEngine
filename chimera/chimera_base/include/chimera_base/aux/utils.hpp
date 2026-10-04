#pragma once
#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace ce {

    [[clang::always_inline]]
    inline const int16_t dead16(const int16_t& v_in, const int16_t& deadzone) {
        return (v_in + (v_in >> 16) ^ (v_in >> 16)) > deadzone ? v_in : 0;
    }

    [[clang::always_inline]]
    inline const float scale16(const int16_t& value, const int16_t& limit) {
        return value >= 0 ? (float)value / (limit - 1) : (float)value / limit;
    }

    [[clang::always_inline]]
    inline const float axis16(const int16_t& v_in, const int16_t& deadzone, const int16_t& limit) {
        return scale16(dead16(v_in, deadzone), limit);
    }

    // TODO: REMOVER
    inline void utilsReadFile(const std::string& filepath, std::string& result) {
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

    inline std::string extractNameByFile(const std::string& filepath) {
        auto last_slash = filepath.find_last_of("/\\");
        last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;

        auto last_dot = filepath.rfind('.');
        auto count = last_dot == std::string::npos ? filepath.size() - last_slash : last_dot - last_slash;
        return filepath.substr(last_slash, count);
    }

    inline void textToStringArray(const std::string& s_in, std::vector<std::string>& v_out, char delimiter) {
        std::string token;
        std::istringstream token_stream(s_in);
        while (std::getline(token_stream, token, delimiter))
            v_out.push_back(token);
    }

    inline void textToFloatArray(const std::string& text, std::vector<float>& array_float) {
        std::vector<std::string> text_data;
        textToStringArray(text, text_data, ' ');
        for (const std::string& val : text_data) {
            if (val.size() != 0)
                array_float.push_back(std::stod(val));
        }
    }

    inline void textToUIntArray(const std::string& text, std::vector<uint32_t>& array_i) {
        std::vector<std::string> text_data;
        textToStringArray(text, text_data, ' ');
        for (const std::string& val : text_data)
            array_i.push_back(static_cast<uint32_t>(std::stoul(val)));
    }
} // namespace ce
