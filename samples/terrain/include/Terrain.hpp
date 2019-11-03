#ifndef __GAME_TERRAIN_TERRAIN__HPP
#define __GAME_TERRAIN_TERRAIN__HPP

#include "BSPTree.hpp"
#include "Frustum.hpp"
//#include "chimera/render/Vertex.hpp"
//#inlude "chimera/render/Load"

namespace Chimera {

class Terrain {
  public:
    Terrain();
    ~Terrain();

    bool LoadTexture2D(char* FileName, float Scale = 256.0f, float Offset = -128.0f);
    bool LoadBinary(char* FileName);
    bool SaveBinary(char* FileName);
    int CheckVisibility(Frustum& _frustum, bool SortVisibleGeometryNodes = true);
    void Render(bool VisualizeRenderingOrder = false);
    void RenderSlow();
    void RenderSlowToShadowMap();
    void RenderAABB(int Depth = -1);
    void Destroy();

    int GetSize();
    glm::vec3 GetMin();
    glm::vec3 GetMax();
    void GetMinMax(glm::mat4& ViewMatrix, glm::vec3& Min, glm::vec3& Max);
    int GetTrianglesCount();
    float GetHeight(float X, float Z);

  private:
    float GetHeight(float* Heights, int Size, float X, float Z);
    int GetIndex(int X, int Z);
    float GetHeight(int X, int Z);
    void SetDefaults();
    int Size, SizeP1;
    float SizeD2;
    glm::vec3 Min, Max;
    float* Heights;
    int VerticesCount, IndicesCount;
    GLuint VertexBufferObject, IndexBufferObject;
    BSPTree bspTree;
};

} // namespace Chimera

#endif