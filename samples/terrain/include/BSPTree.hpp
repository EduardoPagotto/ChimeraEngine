#ifndef __GAME_TERRAIN_BSPTREE__HPP
#define __GAME_TERRAIN_BSPTREE__HPP

#include "BSPTreeNode.hpp"
#include "chimera/core/Vertex.hpp"
#include "chimera/render/Frustum.hpp"

namespace Chimera {

class BSPTree {
  public:
    BSPTree();
    ~BSPTree();

    void Init(VertexData* Vertices, int* Indices, int IndicesCount, const glm::vec3& Min, const glm::vec3& Max,
              float MinAABBSize = 16.0f);
    void QuickSortVisibleGeometryNodes(int Left, int Right);
    int CheckVisibility(Frustum& _frustum, bool SortVisibleGeometryNodes);
    void Render(bool VisualizeRenderingOrder);
    void RenderAABB(int Depth);
    void Destroy();

  private:
    void SetDefaults();

    BSPTreeNode* Root;
    BSPTreeNode** VisibleGeometryNodes;
    int VisibleGeometryNodesCount;
};

} // namespace Chimera

#endif