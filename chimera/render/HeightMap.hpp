#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/render/VertexNode.hpp"

namespace Chimera {

class HeightMap {
  public:
    HeightMap(int _width, int _height, int _squareX, int _squareZ);
    ~HeightMap();

    void split(std::vector<unsigned int> _vVertexIndex);
    void createVertexBuffer(std::vector<VertexData>& _vertexData);
    void render();

  private:
    std::vector<VertexNode*> vNodes;

    int width;
    int height;
    int squareX;
    int squareZ;

    unsigned int vao;
    unsigned int vbo;
};

} // namespace Chimera
#endif
