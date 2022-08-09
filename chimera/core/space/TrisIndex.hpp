#pragma once
#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace Chimera {
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