#include "Terrain.hpp"

namespace Chimera {

Terrain::Terrain() { SetDefaults(); }

Terrain::~Terrain() {}

void Terrain::SetDefaults() {
    Size = 0;
    SizeP1 = 0;
    SizeD2 = 0.0f;

    Min = Max = glm::vec3(0.0f);

    Heights = NULL;

    VerticesCount = 0;
    IndicesCount = 0;

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

// FIXME: REFAZER!!!!!
bool Terrain::LoadBinary(char* FileName) {
    // CString DirectoryFileName = ModuleDirectory + FileName;

    // FILE* File;

    // if (fopen_s(&File, DirectoryFileName, "rb") != 0) {
    //     ErrorLog.Append("Error opening file " + DirectoryFileName + "!\r\n");
    //     return false;
    // }

    // int Size;

    // if (fread(&Size, sizeof(int), 1, File) != 1 || Size <= 0) {
    //     ErrorLog.Append("Error reading file " + DirectoryFileName + "!\r\n");
    //     fclose(File);
    //     return false;
    // }

    // Destroy();

    // this->Size = Size;
    // SizeP1 = Size + 1;
    // SizeD2 = (float)Size / 2.0f;

    // VerticesCount = SizeP1 * SizeP1;

    // Heights = new float[VerticesCount];

    // if (fread(Heights, sizeof(float), VerticesCount, File) != VerticesCount) {
    //     ErrorLog.Append("Error reading file " + DirectoryFileName + "!\r\n");
    //     fclose(File);
    //     Destroy();
    //     return false;
    // }

    // fclose(File);

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

    // int i = 0;

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

// FIXME: refazer
bool Terrain::SaveBinary(char* FileName) {
    // CString DirectoryFileName = ModuleDirectory + FileName;

    // FILE* File;

    // if (fopen_s(&File, DirectoryFileName, "wb+") != 0) {
    //     return false;
    // }

    // fwrite(&Size, sizeof(int), 1, File);

    // fwrite(Heights, sizeof(float), VerticesCount, File);

    // fclose(File);

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

    glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, NULL);

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

    glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::RenderAABB(int Depth) { bspTree.RenderAABB(Depth); }

void Terrain::Destroy() {
    if (Heights != NULL) {
        delete[] Heights;
    }

    if (VertexBufferObject != 0) {
        glDeleteBuffers(1, &VertexBufferObject);
    }

    if (IndexBufferObject != 0) {
        glDeleteBuffers(1, &IndexBufferObject);
    }

    bspTree.Destroy();

    SetDefaults();
}

int Terrain::GetSize() { return Size; }

glm::vec3 Terrain::GetMin() { return Min; }

glm::vec3 Terrain::GetMax() { return Max; }

void Terrain::GetMinMax(glm::mat4& ViewMatrix, glm::vec3& Min, glm::vec3& Max) {
    int i = 0;

    for (int z = 0; z <= Size; z++) {
        for (int x = 0; x <= Size; x++) {
            glm::vec4 Position = ViewMatrix * glm::vec4((float)x - SizeD2, Heights[i], SizeD2 - (float)z, 1.0f);

            if (i == 0) {
                Min.x = Max.x = Position.x;
                Min.y = Max.y = Position.y;
                Min.z = Max.z = Position.z;
            } else {
                if (Position.x < Min.x)
                    Min.x = Position.x;
                if (Position.y < Min.y)
                    Min.y = Position.y;
                if (Position.z < Min.z)
                    Min.z = Position.z;

                if (Position.x > Max.x)
                    Max.x = Position.x;
                if (Position.y > Max.y)
                    Max.y = Position.y;
                if (Position.z > Max.z)
                    Max.z = Position.z;
            }

            i++;
        }
    }
}

int Terrain::GetTrianglesCount() { return IndicesCount / 3; }

int Terrain::GetIndex(int X, int Z) { return SizeP1 * Z + X; }

float Terrain::GetHeight(int X, int Z) {
    return Heights[GetIndex(X < 0 ? 0 : X > Size ? Size : X, Z < 0 ? 0 : Z > Size ? Size : Z)];
}

float Terrain::GetHeight(float X, float Z) {
    Z = -Z;

    X += SizeD2;
    Z += SizeD2;

    float Size = (float)this->Size;

    if (X < 0.0f)
        X = 0.0f;
    if (X > Size)
        X = Size;
    if (Z < 0.0f)
        Z = 0.0f;
    if (Z > Size)
        Z = Size;

    int ix = (int)X, ixp1 = ix + 1;
    int iz = (int)Z, izp1 = iz + 1;

    float fx = X - (float)ix;
    float fz = Z - (float)iz;

    float a = GetHeight(ix, iz);
    float b = GetHeight(ixp1, iz);
    float c = GetHeight(ix, izp1);
    float d = GetHeight(ixp1, izp1);

    float ab = a + (b - a) * fx;
    float cd = c + (d - c) * fx;

    return ab + (cd - ab) * fz;
}

float Terrain::GetHeight(float* Heights, int Size, float X, float Z) {
    float SizeM1F = (float)Size - 1.0f;

    if (X < 0.0f)
        X = 0.0f;
    if (X > SizeM1F)
        X = SizeM1F;
    if (Z < 0.0f)
        Z = 0.0f;
    if (Z > SizeM1F)
        Z = SizeM1F;

    int ix = (int)X, ixp1 = ix + 1;
    int iz = (int)Z, izp1 = iz + 1;

    int SizeM1 = Size - 1;

    if (ixp1 > SizeM1)
        ixp1 = SizeM1;
    if (izp1 > SizeM1)
        izp1 = SizeM1;

    float fx = X - (float)ix;
    float fz = Z - (float)iz;

    int izMSize = iz * Size, izp1MSize = izp1 * Size;

    float a = Heights[izMSize + ix];
    float b = Heights[izMSize + ixp1];
    float c = Heights[izp1MSize + ix];
    float d = Heights[izp1MSize + ixp1];

    float ab = a + (b - a) * fx;
    float cd = c + (d - c) * fx;

    return ab + (cd - ab) * fz;
}

} // namespace Chimera