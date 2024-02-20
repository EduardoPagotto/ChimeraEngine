#pragma once

#include <string.h>

struct Token {
    std::string path;
    std::string type;
    std::string ns;
    std::string name;
    void* value;
    Token* parent;
};