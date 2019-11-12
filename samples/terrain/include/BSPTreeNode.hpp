#ifndef __GAME_TERRAIN_BSPTREENODE__HPP
#define __GAME_TERRAIN_BSPTREENODE__HPP

#include "chimera/render/AABB.hpp"
#include "chimera/render/Frustum.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/render/Vertex.hpp"

namespace Chimera {

class BSPTreeNode {
  public:
    BSPTreeNode();
    ~BSPTreeNode();

    void InitAABB(const glm::vec3& Min, const glm::vec3& Max, int Depth, float MinAABBSize);
    bool CheckTriangle(VertexData* Vertices, int* Indices, int A, int B, int C);
    void AllocateMemory();
    bool AddTriangle(VertexData* Vertices, int* Indices, int A, int B, int C);
    void ResetAABB(VertexData* Vertices);
    int InitIndexBufferObject();
    int CheckVisibility(Frustum& Frustum, BSPTreeNode** VisibleGeometryNodes, int& VisibleGeometryNodesCount);
    float GetDistance();
    void Render();
    void RenderAABB(int Depth);
    void Destroy();

  private:
    void SetDefaults();
    glm::vec3 Min, Max;
    int Depth;
    AABB aabb;
    bool Visible;
    float Distance;
    int* Indices;
    int IndicesCount;
    GLuint IndexBufferObject;
    BSPTreeNode* Children[2];
};

} // namespace Chimera

#endif