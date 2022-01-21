#pragma once
#include "VertexBuffer.hpp"

namespace Chimera {

class VertexArray {
  public:
    VertexArray();
    virtual ~VertexArray();
    void push(VertexBuffer* buffer);
    void bind() const;
    static void unbind();
    inline VertexBuffer* getBuffer(const uint32_t& index) const { return vbos[index]; }
    inline VertexBuffer* getLast() const { return vbos.back(); }

  private:
    uint32_t arrayID;
    std::vector<VertexBuffer*> vbos;
};
} // namespace Chimera
