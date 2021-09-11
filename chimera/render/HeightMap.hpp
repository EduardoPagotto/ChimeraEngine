#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/render/vbs/VertexNode.hpp"

namespace Chimera {

class HeightMap {
  public:
    HeightMap(int _width, int _height, int _squareX, int _squareZ);
    ~HeightMap();

    void split(std::vector<unsigned int> _vVertexIndex);
    void createVertexBuffer(std::vector<VertexData>& _vertexData);
    void render(Frustum& _frustrun);

  private:
    Core::VertexArray* vao;
    std::vector<VertexNode*> vNodes;

    int width;
    int height;
    int squareX;
    int squareZ;
};

} // namespace Chimera
#endif
