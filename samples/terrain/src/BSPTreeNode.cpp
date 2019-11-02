#include "BSPTreeNode.hpp"

namespace Chimera {

BSPTreeNode::BSPTreeNode() { SetDefaults(); }

BSPTreeNode::~BSPTreeNode() {}

void BSPTreeNode::SetDefaults() {
    Min = Max = glm::vec3(0.0f);

    Depth = 0;

    Indices = NULL;

    IndicesCount = 0;

    IndexBufferObject = 0;

    Children[0] = NULL;
    Children[1] = NULL;
}

void BSPTreeNode::InitAABB(const glm::vec3& Min, const glm::vec3& Max, int Depth, float MinAABBSize) {
    this->Min = Min;
    this->Max = Max;

    this->Depth = Depth;

    glm::vec3 Mid = (Min + Max) / 2.0f;
    glm::vec3 Size = Max - Min;

    aabb.set(Min, Max);

    if (Size.x > MinAABBSize || Size.z > MinAABBSize) {
        Children[0] = new BSPTreeNode();
        Children[1] = new BSPTreeNode();

        if (Size.x >= Size.z) {
            Children[0]->InitAABB(glm::vec3(Min.x, Min.y, Min.z), glm::vec3(Mid.x, Max.y, Max.z), Depth + 1,
                                  MinAABBSize);
            Children[1]->InitAABB(glm::vec3(Mid.x, Min.y, Min.z), glm::vec3(Max.x, Max.y, Max.z), Depth + 1,
                                  MinAABBSize);
        } else {
            Children[0]->InitAABB(glm::vec3(Min.x, Min.y, Min.z), glm::vec3(Max.x, Max.y, Mid.z), Depth + 1,
                                  MinAABBSize);
            Children[1]->InitAABB(glm::vec3(Min.x, Min.y, Mid.z), glm::vec3(Max.x, Max.y, Max.z), Depth + 1,
                                  MinAABBSize);
        }
    }
}

bool BSPTreeNode::CheckTriangle(VertexData* Vertices, int* Indices, int A, int B, int C) {
    if (aabb.pointInside(Vertices[Indices[A]].position)) {
        if (aabb.pointInside(Vertices[Indices[B]].position)) {
            if (aabb.pointInside(Vertices[Indices[C]].position)) {
                bool BelongsToAChild = false;

                if (Children[0] != NULL) {
                    BelongsToAChild |= Children[0]->CheckTriangle(Vertices, Indices, A, B, C);
                }

                if (Children[1] != NULL && !BelongsToAChild) {
                    BelongsToAChild |= Children[1]->CheckTriangle(Vertices, Indices, A, B, C);
                }

                if (!BelongsToAChild) {
                    IndicesCount += 3;
                }

                return true;
            }
        }
    }

    return false;
}

void BSPTreeNode::AllocateMemory() {
    if (IndicesCount > 0) {
        Indices = new int[IndicesCount];
        IndicesCount = 0;
    }

    if (Children[0] != NULL) {
        Children[0]->AllocateMemory();
    }

    if (Children[1] != NULL) {
        Children[1]->AllocateMemory();
    }
}

bool BSPTreeNode::AddTriangle(VertexData* Vertices, int* Indices, int A, int B, int C) {
    if (aabb.pointInside(Vertices[Indices[A]].position)) {
        if (aabb.pointInside(Vertices[Indices[B]].position)) {
            if (aabb.pointInside(Vertices[Indices[C]].position)) {
                bool BelongsToAChild = false;

                if (Children[0] != NULL) {
                    BelongsToAChild |= Children[0]->AddTriangle(Vertices, Indices, A, B, C);
                }

                if (Children[1] != NULL && !BelongsToAChild) {
                    BelongsToAChild |= Children[1]->AddTriangle(Vertices, Indices, A, B, C);
                }

                if (!BelongsToAChild) {
                    this->Indices[IndicesCount++] = Indices[A];
                    this->Indices[IndicesCount++] = Indices[B];
                    this->Indices[IndicesCount++] = Indices[C];
                }

                return true;
            }
        }
    }

    return false;
}

void BSPTreeNode::ResetAABB(VertexData* Vertices) {
    float MinY = Min.y, MaxY = Max.y;

    Min.y = MaxY;
    Max.y = MinY;

    if (IndicesCount > 0) {
        for (int i = 0; i < IndicesCount; i++) {
            if (Vertices[Indices[i]].position.y < Min.y)
                Min.y = Vertices[Indices[i]].position.y;
            if (Vertices[Indices[i]].position.y > Max.y)
                Max.y = Vertices[Indices[i]].position.y;
        }
    }

    if (Children[0] != NULL) {
        Children[0]->ResetAABB(Vertices);

        if (Children[0]->Min.y < Min.y)
            Min.y = Children[0]->Min.y;
        if (Children[0]->Max.y > Max.y)
            Max.y = Children[0]->Max.y;
    }

    if (Children[1] != NULL) {
        Children[1]->ResetAABB(Vertices);

        if (Children[1]->Min.y < Min.y)
            Min.y = Children[1]->Min.y;
        if (Children[1]->Max.y > Max.y)
            Max.y = Children[1]->Max.y;
    }

    aabb.set(Min, Max);
}

int BSPTreeNode::InitIndexBufferObject() {
    int GeometryNodesCount = 0;

    if (IndicesCount > 0) {
        glGenBuffers(1, &IndexBufferObject);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(int), Indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        delete[] Indices;
        Indices = NULL;

        GeometryNodesCount++;
    }

    if (Children[0] != NULL) {
        GeometryNodesCount += Children[0]->InitIndexBufferObject();
    }

    if (Children[1] != NULL) {
        GeometryNodesCount += Children[1]->InitIndexBufferObject();
    }

    return GeometryNodesCount;
}

int BSPTreeNode::CheckVisibility(Frustum& Frustum, BSPTreeNode** VisibleGeometryNodes, int& VisibleGeometryNodesCount) {
    int TrianglesRendered = 0;

    Visible = aabb.visible(Frustum);

    if (Visible) {
        if (IndicesCount > 0) {
            Distance = aabb.distance(Frustum);

            VisibleGeometryNodes[VisibleGeometryNodesCount++] = this;

            TrianglesRendered += IndicesCount / 3;
        }

        if (Children[0] != NULL) {
            TrianglesRendered += Children[0]->CheckVisibility(Frustum, VisibleGeometryNodes, VisibleGeometryNodesCount);
        }

        if (Children[1] != NULL) {
            TrianglesRendered += Children[1]->CheckVisibility(Frustum, VisibleGeometryNodes, VisibleGeometryNodesCount);
        }
    }

    return TrianglesRendered;
}

float BSPTreeNode::GetDistance() { return Distance; }

void BSPTreeNode::Render() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);

    glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BSPTreeNode::RenderAABB(int Depth) {
    if (Visible) {
        if (Depth == -1 || Depth == this->Depth) {
            aabb.render();
        }

        if (Children[0] != NULL) {
            Children[0]->RenderAABB(Depth);
        }

        if (Children[1] != NULL) {
            Children[1]->RenderAABB(Depth);
        }
    }
}

void BSPTreeNode::Destroy() {
    if (Indices != NULL) {
        delete[] Indices;
    }

    if (IndexBufferObject != 0) {
        glDeleteBuffers(1, &IndexBufferObject);
    }

    if (Children[0] != NULL) {
        Children[0]->Destroy();
        delete Children[0];
    }

    if (Children[1] != NULL) {
        Children[1]->Destroy();
        delete Children[1];
    }

    SetDefaults();
}

} // namespace Chimera