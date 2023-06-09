#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Triangle {
    Triangle(const Triangle& t) : point(t.point), normal(t.normal), splitter(t.splitter) {}
    Triangle(const glm::uvec3& t, const glm::vec3& _normal, const bool& split) : normal(_normal), splitter(split) { this->point = t; }
    bool splitter;
    glm::uvec3 point;
    glm::vec3 normal;
};

struct TrisIndex {
    TrisIndex() = default;
    TrisIndex(const TrisIndex& o) : vIndex(o.vIndex) {}
    virtual ~TrisIndex() { vIndex.clear(); }
    inline void add(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
        vIndex.push_back(_pa);
        vIndex.push_back(_pb);
        vIndex.push_back(_pc);
    }
    inline const size_t size() const { return vIndex.size(); }
    std::vector<uint32_t> vIndex;
};
// using VecPrtTrisIndex = std::vector<TrisIndex>;

} // namespace Chimera
