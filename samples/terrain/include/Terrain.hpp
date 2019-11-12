#ifndef __GAME_TERRAIN_TERRAIN__HPP
#define __GAME_TERRAIN_TERRAIN__HPP

#include "BSPTree.hpp"
#include "chimera/render/Frustum.hpp"
#include "chimera/render/TerrainData.hpp"

namespace Chimera {

class Terrain {
  public:
    Terrain();
    ~Terrain();

    bool LoadTexture2D(char* FileName, float Scale = 256.0f, float Offset = -128.0f);
    bool LoadBinary(char* FileName);
    bool SaveBinary(char* FileName) { return terrain.saveBinary(FileName); }
    int CheckVisibility(Frustum& _frustum, bool SortVisibleGeometryNodes = true);
    void Render(bool VisualizeRenderingOrder = false);
    void RenderSlow();
    void RenderSlowToShadowMap();
    void RenderAABB(int Depth = -1);
    void Destroy();

    int GetSize() { return terrain.getSize(); }
    glm::vec3 GetMin() { return terrain.getMin(); }
    glm::vec3 GetMax() { return terrain.getMax(); }
    void GetMinMax(glm::mat4& ViewMatrix, glm::vec3& Min, glm::vec3& Max) { terrain.getMinMax(ViewMatrix, Min, Max); }
    int GetTrianglesCount() { return terrain.getTrianglesCount(); }
    float GetHeight(float X, float Z) { return terrain.getHeight(X, Z); }

  private:
    Chimera::TerrainData terrain;
    void SetDefaults();
    GLuint VertexBufferObject, IndexBufferObject;
    BSPTree bspTree;
};

} // namespace Chimera

#endif