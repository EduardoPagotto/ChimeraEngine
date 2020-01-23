#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "VertexBuffer.hpp"

namespace Chimera {

class VertexRenderDynamic : public VertexBuffer {
  public:
    VertexRenderDynamic();
    void create(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    virtual void createIndex() override{};
    virtual void clearIndex() override{};
};
} // namespace Chimera
#endif