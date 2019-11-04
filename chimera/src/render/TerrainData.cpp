#include "chimera/render/TerrainData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

TerrainData::TerrainData() { setDefaults(); }
TerrainData::~TerrainData() { destroy(); }

void TerrainData::setDefaults() {
    sizeHeight = 0;
    sizeP1 = 0;
    sizeD2 = 0.0f;
    Min = Max = glm::vec3(0.0f);
    heights = nullptr;
}

void TerrainData::destroy() {

    if (heights != nullptr)
        delete[] heights;

    // if (heights.size() > 0)
    //     heights.clear();

    if (vertices.size() > 0)
        vertices.clear();

    if (indices.size() > 0)
        indices.clear();

    setDefaults();
}

int TerrainData::getIndex(int X, int Z) { return sizeP1 * Z + X; }

float TerrainData::getHeight(int X, int Z) {

    int x1 = X < 0 ? 0 : X > sizeHeight ? sizeHeight : X;
    int z1 = Z < 0 ? 0 : Z > sizeHeight ? sizeHeight : Z;
    int pos = getIndex(x1, z1);

    return heights[pos];
}

bool TerrainData::saveBinary(char* FileName) {

    FILE* File = fopen(FileName, (char*)"wb+");
    if (File == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        return false;
    }

    fwrite(&this->sizeHeight, sizeof(int), 1, File);
    fwrite(heights, sizeof(float), verticesCount, File);
    fclose(File);

    return true;
}

// FIXME: trocat por tex
bool TerrainData::loadTexture2D(char* FileName, float Scale, float Offset) {
    // CTexture Texture;

    // if (!Texture.LoadTexture2D(FileName)) {
    //     return false;
    // }

    // if (Texture.GetWidth() != Texture.getHeight()) {
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
    //         Heights[i++] = getHeight(TextureHeights, Size, (float)x - 0.5f, (float)z - 0.5f);
    //     }
    // }

    // delete[] TextureHeights;

    // float* SmoothedHeights = new float[VerticesCount];

    // i = 0;

    // for (int z = 0; z <= Size; z++) {
    //     for (int x = 0; x <= Size; x++) {
    //         SmoothedHeights[i] = 0.0f;

    //         SmoothedHeights[i] += getHeight(x - 1, z + 1) + getHeight(x, z + 1) * 2 + getHeight(x + 1, z + 1);
    //         SmoothedHeights[i] += getHeight(x - 1, z) * 2 + getHeight(x, z) * 3 + getHeight(x + 1, z) * 2;
    //         SmoothedHeights[i] += getHeight(x - 1, z - 1) + getHeight(x, z - 1) * 2 + getHeight(x + 1, z - 1);

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
    //             glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z), 2.0f, getHeight(x, z + 1) - getHeight(x, z -
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

    // FIXME: MUDAR PARA TRUE QUANDO IMPLEMENTAR ESTE METODO
    return false;
}

bool TerrainData::loadBinary(char* FileName) {

    FILE* File = fopen(FileName, (char*)"rb");
    if (File == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        return false;
    }

    int Size;

    if (fread(&Size, sizeof(int), 1, File) != 1 || Size <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        return false;
    }

    destroy();

    this->sizeHeight = Size;
    sizeP1 = Size + 1;
    sizeD2 = (float)Size / 2.0f;

    verticesCount = sizeP1 * sizeP1;

    // heights.reserve(verticesCount);
    heights = new float[verticesCount];

    // FIXME: melhorar esta horrivel, carga de vector deve ser direta do arquivo
    // float* h = new float[verticesCount];
    size_t tam = fread(heights, sizeof(float), verticesCount, File);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        destroy();
        return false;
    }
    // for (int i = 0; i < verticesCount; i++) {
    //     heights.push_back(h[i]);
    // }
    // delete[] h;

    fclose(File);

    Min.x = Min.z = -sizeD2;
    Max.x = Max.z = sizeD2;

    Min.y = Max.y = heights[0];

    for (int i = 1; i < verticesCount; i++) {
        if (heights[i] < Min.y)
            Min.y = heights[i];
        if (heights[i] > Max.y)
            Max.y = heights[i];
    }

    vertices.reserve(verticesCount);

    int i = 0;

    for (int z = 0; z <= Size; z++) {
        for (int x = 0; x <= Size; x++) {
            glm::vec3 pos = glm::vec3((float)x - sizeD2,  // posx
                                      heights[i],         // posx
                                      sizeD2 - (float)z); // posx

            glm::vec3 nor = glm::normalize(glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     getHeight(x, z + 1) - getHeight(x, z - 1))); // norz

            vertices.push_back({pos, nor, glm::vec2(0.0, 0.0)});
            i++;
        }
    }

    int indicesCount = Size * Size * 2 * 3;
    indices.reserve(indicesCount);

    i = 0;

    for (int z = 0; z < Size; z++) {
        for (int x = 0; x < Size; x++) {

            indices.push_back(getIndex(x, z));
            indices.push_back(getIndex(x + 1, z));
            indices.push_back(getIndex(x + 1, z + 1));

            indices.push_back(getIndex(x + 1, z + 1));
            indices.push_back(getIndex(x, z + 1));
            indices.push_back(getIndex(x, z));
        }
    }

    return true;
}

void TerrainData::getMinMax(glm::mat4& ViewMatrix, glm::vec3& Min, glm::vec3& Max) {
    int i = 0;

    for (int z = 0; z <= sizeHeight; z++) {
        for (int x = 0; x <= sizeHeight; x++) {
            glm::vec4 Position = ViewMatrix * glm::vec4((float)x - sizeD2, heights[i], sizeD2 - (float)z, 1.0f);

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

float TerrainData::getHeight(float X, float Z) {
    Z = -Z;

    X += sizeD2;
    Z += sizeD2;

    float Size = (float)this->sizeHeight;

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

    float a = getHeight(ix, iz);
    float b = getHeight(ixp1, iz);
    float c = getHeight(ix, izp1);
    float d = getHeight(ixp1, izp1);

    float ab = a + (b - a) * fx;
    float cd = c + (d - c) * fx;

    return ab + (cd - ab) * fz;
}

// FIXME: ver se rola!!
float TerrainData::getHeight(float* Heights, int Size, float X, float Z) {
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