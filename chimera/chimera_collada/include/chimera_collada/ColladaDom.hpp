#pragma once
#include <pugixml.hpp>

namespace ce {
struct ColladaDom {
    ColladaDom() = default;
    ColladaDom(const ColladaDom& other) = default;
    std::string file = "";
    pugi::xml_node root = pugi::xml_node();
    pugi::xml_document* pDoc = nullptr;
};
} // namespace ce