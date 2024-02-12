#pragma once
#include "VertexBuffer.hpp"

namespace Chimera {

class VertexArray {
  private:
    uint32_t arrayID = 0;
    std::vector<VertexBuffer*> vbos;

  public:
    VertexArray();
    virtual ~VertexArray();
    void bind() const;
    static void unbind();
    inline void push(VertexBuffer* buffer) { this->vbos.push_back(buffer); }
    inline VertexBuffer* getBuffer(const uint32_t& index) const { return vbos[index]; }
    inline VertexBuffer* getLast() const { return vbos.back(); }
};
} // namespace Chimera
