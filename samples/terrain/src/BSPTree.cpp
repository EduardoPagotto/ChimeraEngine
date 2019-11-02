#include "BSPTree.hpp"

namespace Chimera {

BSPTree::BSPTree() { SetDefaults(); }

BSPTree::~BSPTree() {}

void BSPTree::SetDefaults() {
    Root = NULL;

    VisibleGeometryNodes = NULL;
}

void BSPTree::Init(VertexData* Vertices, int* Indices, int IndicesCount, const glm::vec3& Min, const glm::vec3& Max,
                   float MinAABBSize) {
    Destroy();

    if (Vertices != NULL && Indices != NULL && IndicesCount > 0) {
        Root = new BSPTreeNode();

        Root->InitAABB(Min, Max, 0, MinAABBSize);

        for (int i = 0; i < IndicesCount; i += 3) {
            Root->CheckTriangle(Vertices, Indices, i, i + 1, i + 2);
        }

        Root->AllocateMemory();

        for (int i = 0; i < IndicesCount; i += 3) {
            Root->AddTriangle(Vertices, Indices, i, i + 1, i + 2);
        }

        Root->ResetAABB(Vertices);

        int GeometryNodesCount = Root->InitIndexBufferObject();

        VisibleGeometryNodes = new BSPTreeNode*[GeometryNodesCount];
    }
}

void BSPTree::QuickSortVisibleGeometryNodes(int Left, int Right) {
    float Pivot = VisibleGeometryNodes[(Left + Right) / 2]->GetDistance();
    int i = Left, j = Right;

    while (i <= j) {
        while (VisibleGeometryNodes[i]->GetDistance() < Pivot)
            i++;
        while (VisibleGeometryNodes[j]->GetDistance() > Pivot)
            j--;

        if (i <= j) {
            if (i != j) {
                BSPTreeNode* Temp = VisibleGeometryNodes[i];
                VisibleGeometryNodes[i] = VisibleGeometryNodes[j];
                VisibleGeometryNodes[j] = Temp;
            }

            i++;
            j--;
        }
    }

    if (Left < j) {
        QuickSortVisibleGeometryNodes(Left, j);
    }

    if (i < Right) {
        QuickSortVisibleGeometryNodes(i, Right);
    }
}

int BSPTree::CheckVisibility(Frustum& _frustum, bool SortVisibleGeometryNodes) {
    int TrianglesRendered = 0;

    VisibleGeometryNodesCount = 0;

    if (Root != NULL) {
        TrianglesRendered = Root->CheckVisibility(_frustum, VisibleGeometryNodes, VisibleGeometryNodesCount);

        if (SortVisibleGeometryNodes) {
            if (VisibleGeometryNodesCount > 1) {
                QuickSortVisibleGeometryNodes(0, VisibleGeometryNodesCount - 1);
            }
        }
    }

    return TrianglesRendered;
}

void BSPTree::Render(bool VisualizeRenderingOrder) {
    if (VisibleGeometryNodesCount > 0) {
        if (!VisualizeRenderingOrder) {
            for (int i = 0; i < VisibleGeometryNodesCount; i++) {
                VisibleGeometryNodes[i]->Render();
            }
        } else {
            for (int i = 0; i < VisibleGeometryNodesCount; i++) {
                float Color = (float)(i + 1) / (float)VisibleGeometryNodesCount;

                glColor3f(Color, Color, Color);

                VisibleGeometryNodes[i]->Render();
            }
        }
    }
}

void BSPTree::RenderAABB(int Depth) {
    if (Root != NULL) {
        Root->RenderAABB(Depth);
    }
}

void BSPTree::Destroy() {
    if (Root != NULL) {
        Root->Destroy();
        delete Root;
    }

    if (VisibleGeometryNodes != NULL) {
        delete[] VisibleGeometryNodes;
    }

    SetDefaults();
}

} // namespace Chimera