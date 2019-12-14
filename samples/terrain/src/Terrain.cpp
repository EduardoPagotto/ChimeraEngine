#include "Terrain.hpp"

namespace Chimera {

Terrain::Terrain() { SetDefaults(); }

Terrain::~Terrain() {}

void Terrain::SetDefaults() {
    terrain.setDefaults();
    VertexBufferObject = 0;
    IndexBufferObject = 0;
}

bool Terrain::LoadTexture2D(char* FileName, float Scale, float Offset) {
    return terrain.loadTexture2D(FileName, Scale, Offset);
}

bool Terrain::LoadBinary(char* FileName) {

    bool okLoad = terrain.loadBinary(FileName);

    unsigned int sizeBufferVertex = terrain.vertices.size() * sizeof(VertexData);
    unsigned int sizeBufferIndex = terrain.indices.size() * sizeof(unsigned int);

    glGenBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &terrain.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &IndexBufferObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &terrain.indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // TODO: TESTAR SE VALORES PASSAM!!!!!
    bspTree.Init(&terrain.vertices[0], &terrain.indices[0], terrain.indices.size(), terrain.getMin(), terrain.getMax());

    return true;
}

int Terrain::CheckVisibility(Frustum& _frustum, bool SortVisibleGeometryNodes) {
    return bspTree.CheckVisibility(_frustum, SortVisibleGeometryNodes);
}

void Terrain::Render(bool VisualizeRenderingOrder) {
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 0));

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 1));

    bspTree.Render(VisualizeRenderingOrder);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::RenderSlow() {
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 0));

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 1));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);

    glDrawElements(GL_TRIANGLES, terrain.indices.size(), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::RenderSlowToShadowMap() {
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);

    glDrawElements(GL_TRIANGLES, terrain.indices.size(), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::RenderAABB(int Depth) { bspTree.RenderAABB(Depth); }

void Terrain::Destroy() {
    terrain.destroy();

    if (VertexBufferObject != 0) {
        glDeleteBuffers(1, &VertexBufferObject);
    }

    if (IndexBufferObject != 0) {
        glDeleteBuffers(1, &IndexBufferObject);
    }

    bspTree.Destroy();
}
} // namespace Chimera