#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/render/VertexBuffer.hpp"
#include "chimera/render/VertexNode.hpp"

namespace Chimera {

class HeightMap : public VertexBuffer {
  public:
    HeightMap(int _width, int _height, int _squareX, int _squareZ);
    ~HeightMap();

    void split(std::vector<unsigned int> _vVertexIndex);
    void createVertexBuffer(std::vector<VertexData>& _vertexData);
    void render();

  private:
    virtual void createIndex() override;
    virtual void clearIndex() override;

  private:
    std::vector<VertexNode*> vNodes;

    int width;
    int height;
    int squareX;
    int squareZ;
};

} // namespace Chimera
#endif
