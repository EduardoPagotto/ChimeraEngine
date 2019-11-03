#include "Terrain.hpp"

namespace Chimera {

Terrain::Terrain() { SetDefaults(); }

Terrain::~Terrain() {}

void Terrain::SetDefaults() {
    terrain.setDefaults();
    VertexBufferObject = 0;
    IndexBufferObject = 0;
}

// FIXME: trocat por tex
bool Terrain::LoadTexture2D(char* FileName, float Scale, float Offset) {
    // CTexture Texture;

    // if (!Texture.LoadTexture2D(FileName)) {
    //     return false;
    // }

    // if (Texture.GetWidth() != Texture.GetHeight()) {
    //     ErrorLog.Append("Unsupported texture dimensions (%s)!\r\n", FileName);
    //     Texture.Destroy();
    //     return false;
    // }

    // Destroy();

    // Size = Texture.GetWidth();
    // SizeP1 = Size + 1;
    // SizeD2 = (float)Size / 2.0f;

    // VerticesCount = SizeP1 * SizeP1;

    // float* TextureHeights = new float[Size * Size];

    // glBindTexture(GL_TEXTURE_2D, Texture);
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_FLOAT, TextureHeights);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // Texture.Destroy();

    // for (int i = 0; i < Size * Size; i++) {
    //     TextureHeights[i] = TextureHeights[i] * Scale + Offset;
    // }

    // Heights = new float[VerticesCount];

    // int i = 0;

    // for (int z = 0; z <= Size; z++) {
    //     for (int x = 0; x <= Size; x++) {
    //         Heights[i++] = GetHeight(TextureHeights, Size, (float)x - 0.5f, (float)z - 0.5f);
    //     }
    // }

    // delete[] TextureHeights;

    // float* SmoothedHeights = new float[VerticesCount];

    // i = 0;

    // for (int z = 0; z <= Size; z++) {
    //     for (int x = 0; x <= Size; x++) {
    //         SmoothedHeights[i] = 0.0f;

    //         SmoothedHeights[i] += GetHeight(x - 1, z + 1) + GetHeight(x, z + 1) * 2 + GetHeight(x + 1, z + 1);
    //         SmoothedHeights[i] += GetHeight(x - 1, z) * 2 + GetHeight(x, z) * 3 + GetHeight(x + 1, z) * 2;
    //         SmoothedHeights[i] += GetHeight(x - 1, z - 1) + GetHeight(x, z - 1) * 2 + GetHeight(x + 1, z - 1);

    //         SmoothedHeights[i] /= 15.0f;

    //         i++;
    //     }
    // }

    // delete[] Heights;

    // Heights = SmoothedHeights;

    // Min.x = Min.z = -SizeD2;
    // Max.x = Max.z = SizeD2;

    // Min.y = Max.y = Heights[0];

    // for (int i = 1; i < VerticesCount; i++) {
    //     if (Heights[i] < Min.y)
    //         Min.y = Heights[i];
    //     if (Heights[i] > Max.y)
    //         Max.y = Heights[i];
    // }

    // VertexData* Vertices = new VertexData[VerticesCount];

    // i = 0;

    // for (int z = 0; z <= Size; z++) {
    //     for (int x = 0; x <= Size; x++) {
    //         Vertices[i].position = glm::vec3((float)x - SizeD2, Heights[i], SizeD2 - (float)z);
    //         Vertices[i].normal = normalize(
    //             glm::vec3(GetHeight(x - 1, z) - GetHeight(x + 1, z), 2.0f, GetHeight(x, z + 1) - GetHeight(x, z -
    //             1)));

    //         i++;
    //     }
    // }

    // glGenBuffers(1, &VertexBufferObject);

    // glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    // glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(VertexData), Vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // IndicesCount = Size * Size * 2 * 3;

    // int* Indices = new int[IndicesCount];

    // i = 0;

    // for (int z = 0; z < Size; z++) {
    //     for (int x = 0; x < Size; x++) {
    //         Indices[i++] = GetIndex(x, z);
    //         Indices[i++] = GetIndex(x + 1, z);
    //         Indices[i++] = GetIndex(x + 1, z + 1);

    //         Indices[i++] = GetIndex(x + 1, z + 1);
    //         Indices[i++] = GetIndex(x, z + 1);
    //         Indices[i++] = GetIndex(x, z);
    //     }
    // }

    // glGenBuffers(1, &IndexBufferObject);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount * sizeof(int), Indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // bspTree.Init(Vertices, Indices, IndicesCount, Min, Max);

    // delete[] Vertices;
    // delete[] Indices;

    return true;
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