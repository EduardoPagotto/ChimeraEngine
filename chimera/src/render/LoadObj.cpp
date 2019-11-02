#include "chimera/render/LoadObj.hpp"
#include "chimera/core/Exception.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

void eraseAllSubStr(std::string& mainStr, const std::string& toErase) {
    size_t pos = std::string::npos;
    while ((pos = mainStr.find(toErase)) != std::string::npos) {
        mainStr.erase(pos, toErase.length());
    }
}

bool getValidData(std::string& nova, const std::string& comando) {
    std::string fim_linha("\n");
    std::size_t found = nova.find(comando);
    if (found != std::string::npos) {
        eraseAllSubStr(nova, comando);
        eraseAllSubStr(nova, fim_linha);
        return true;
    }
    return false;
}

void loadObjMtl(const std::string& _fineNameMtl, MeshData& _mesh, Material& _pMaterial) {
    std::string matFile;
    loadObj(_fineNameMtl, _mesh, matFile);

    if (matFile.size() > 0)
        loadMtl(matFile, &_pMaterial);
}

void loadMtl(const std::string& _fineNameMtl, Material* _pMaterial) {
    FILE* fp = fopen(_fineNameMtl.c_str(), "r");
    if (fp == NULL)
        throw Exception("Erro ao abrir arquivo: " + _fineNameMtl);

    char* line = NULL;
    size_t len = 0;
    unsigned int pos_linha = 0;
    while ((getline(&line, &len, fp)) != -1) {

        float r, g, b, a;
        pos_linha++;

        if (line[0] == '#')
            continue;
        else if (line[0] == 'K') {

            if (line[1] == 'a') {
                int n = sscanf(line, "Ka %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _pMaterial->setAmbient(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _pMaterial->setAmbient(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo: " + _fineNameMtl);
                }

            } else if (line[1] == 'd') {
                int n = sscanf(line, "Kd %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _pMaterial->setDiffuse(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _pMaterial->setDiffuse(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo: " + _fineNameMtl);
                }
            } else if (line[1] == 's') {
                int n = sscanf(line, "Ks %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _pMaterial->setSpecular(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _pMaterial->setSpecular(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo:" + _fineNameMtl);
                }
            }
        } else {
            // load texture
            std::string nova(line);
            if (getValidData(nova, std::string("map_Kd ")) == true)
                _pMaterial->addTexture(new TexImg(TEX_KIND::DIFFUSE, nova));
        }
    }
}

void loadObj(const std::string& _fineNameObj, MeshData& _mesh, std::string& _materialFile) {

    _materialFile = "";
    FILE* fp = fopen(_fineNameObj.c_str(), "r");
    if (fp == NULL)
        throw Exception("Erro ao abrir arquivo: " + _fineNameObj);

    bool textuaOn = false;
    bool normalOn = false;
    char* line = NULL;
    size_t len = 0;
    unsigned int pos_linha = 0;
    while ((getline(&line, &len, fp)) != -1) {
        float x, y, z, u, v;
        int A[3];
        int B[3];
        int C[3];

        pos_linha++;

        if (line[0] == '#')
            continue;
        else if (line[0] == 'v') {
            if (line[1] == ' ') {
                if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                    _mesh.vertexList.push_back(glm::vec3(x, y, z));
                    continue;
                }
                throw Exception(
                    std::string("linha " + std::to_string(pos_linha) + " parse invalido arquivo: " + _fineNameObj));

            } else if (line[1] == 'n') {
                normalOn = true;
                int n = sscanf(line, "vn %f %f %f", &x, &y, &z);
                _mesh.normalList.push_back(glm::vec3(x, y, z));
            } else if (line[1] == 't') {
                textuaOn = true;
                int n = sscanf(line, "vt %f %f", &u, &v);
                _mesh.textureList.push_back(glm::vec2(u, v));
            }
        } else if (line[0] == 'f') {
            if (line[1] == ' ') {
                if ((textuaOn == true) && (normalOn == true)) {
                    int n = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &A[0], &B[0], &C[0], &A[1], &B[1], &C[1],
                                   &A[2], &B[2], &C[2]);
                } else if ((textuaOn == false) && (normalOn == true)) {
                    int n = sscanf(line, "f %d//%d %d//%d %d//%d", &A[0], &C[0], &A[1], &C[1], &A[2], &C[2]);
                } else if ((textuaOn == true) && (normalOn == false)) {
                    int n = sscanf(line, "f %d/%d %d/%d %d/%d", &A[0], &B[0], &A[1], &B[1], &A[2], &B[2]);
                } else {
                    int n = sscanf(line, "f %d %d %d", &A[0], &A[1], &A[2]);
                }

                for (int indice = 0; indice < 3; indice++) {
                    _mesh.vertexIndex.push_back(A[indice] - 1);

                    if (textuaOn == true)
                        _mesh.textureIndex.push_back(B[indice] - 1);

                    if (normalOn == true)
                        _mesh.normalIndex.push_back(C[indice] - 1);
                }
            }
        } else {
            std::string nova(line);
            if (getValidData(nova, std::string("mtllib ")) == true)
                _materialFile = nova;
        }
    }
    fclose(fp);
    if (line)
        free(line);
}

void convertMeshDataTriangle(MeshData* _pMesh, std::vector<Triangle>& vecTriangle) {

    unsigned int B, C;

    // Load vertex, normal and texture of triangles A,B,C
    for (unsigned int A = 0; A < _pMesh->vertexIndex.size(); A += 3) {
        B = A + 1;
        C = A + 2;

        Triangle t = Triangle(_pMesh->vertexList[_pMesh->vertexIndex[A]],  // vertex triangle A
                              _pMesh->vertexList[_pMesh->vertexIndex[B]],  // vertex triangle B
                              _pMesh->vertexList[_pMesh->vertexIndex[C]]); // vertex triangle C

        if (_pMesh->normalList.size() > 0) {
            t.vertex[0].normal = _pMesh->normalList[_pMesh->normalIndex[A]]; // normal triangle A
            t.vertex[1].normal = _pMesh->normalList[_pMesh->normalIndex[B]]; // normal triangle B
            t.vertex[2].normal = _pMesh->normalList[_pMesh->normalIndex[C]]; // normal triangle C
        }

        if (_pMesh->textureList.size() > 0) {
            t.vertex[0].texture = _pMesh->textureList[_pMesh->textureIndex[A]]; // texture triangle A
            t.vertex[1].texture = _pMesh->textureList[_pMesh->textureIndex[B]]; // texture triangle B
            t.vertex[2].texture = _pMesh->textureList[_pMesh->textureIndex[C]]; // texture triangle C
        }

        vecTriangle.push_back(t);
        // t.debugData();
    }
}

void convertMeshDataVertexData(MeshData* _pMesh, std::vector<VertexData>& outData) {

    unsigned int B, C;
    for (unsigned int A = 0; A < _pMesh->vertexIndex.size(); A += 3) {

        B = A + 1;
        C = A + 2;

        // vertex, normal and texture triangle A
        outData.push_back(
            {_pMesh->vertexList[_pMesh->vertexIndex[A]], _pMesh->normalList[_pMesh->normalIndex[A]],
             (_pMesh->textureList.size() > 0) ? _pMesh->textureList[_pMesh->textureIndex[A]] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back(
            {_pMesh->vertexList[_pMesh->vertexIndex[B]], _pMesh->normalList[_pMesh->normalIndex[B]],
             (_pMesh->textureList.size() > 0) ? _pMesh->textureList[_pMesh->textureIndex[B]] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back(
            {_pMesh->vertexList[_pMesh->vertexIndex[C]], _pMesh->normalList[_pMesh->normalIndex[C]],
             (_pMesh->textureList.size() > 0) ? _pMesh->textureList[_pMesh->textureIndex[C]] : glm::vec2(0.0, 0.0)});
    }
}

void loadTerrain(const std::string& _fileName, MeshData& _mesh) {

    TerrainData t;
    t.LoadBinary((char*)_fileName.c_str());

    for (int i = 0; i < t.vertices.size(); i++) {
        glm::vec3 pos = t.vertices[i].position;
        glm::vec3 nor = t.vertices[i].normal;
        _mesh.vertexList.push_back(pos);
        _mesh.normalList.push_back(nor);
    }

    for (int i = 0; i < t.indices.size(); i++) {
        int val = t.indices[i];
        _mesh.normalIndex.push_back(val);
        _mesh.vertexIndex.push_back(val);
    }
}

//-----------

TerrainData::TerrainData() { SetDefaults(); }
TerrainData::~TerrainData() { Destroy(); }

void TerrainData::SetDefaults() {
    sizeHeight = 0;
    sizeP1 = 0;
    sizeD2 = 0.0f;
    Min = Max = glm::vec3(0.0f);
    // Heights = NULL;
}

void TerrainData::Destroy() {
    if (heights.size() > 0)
        heights.clear();

    if (vertices.size() > 0)
        vertices.clear();

    if (indices.size() > 0)
        indices.clear();

    SetDefaults();
}

int TerrainData::GetIndex(int X, int Z) { return sizeP1 * Z + X; }

float TerrainData::GetHeight(int X, int Z) {

    int x1 = X < 0 ? 0 : X > sizeHeight ? sizeHeight : X;
    int z1 = Z < 0 ? 0 : Z > sizeHeight ? sizeHeight : Z;
    int pos = GetIndex(x1, z1);

    return heights[pos];
}

bool TerrainData::LoadBinary(char* FileName) {

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

    Destroy();

    this->sizeHeight = Size;
    sizeP1 = Size + 1;
    sizeD2 = (float)Size / 2.0f;

    int verticesCount = sizeP1 * sizeP1;

    heights.reserve(verticesCount);

    // FIXME: melhorar esta horrivel, carga de vector deve ser direta do arquivo
    float* h = new float[verticesCount];
    size_t tam = fread(h, sizeof(float), verticesCount, File);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        Destroy();
        return false;
    }
    for (int i = 0; i < verticesCount; i++) {
        heights.push_back(h[i]);
    }
    delete[] h;

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

            glm::vec3 nor = glm::normalize(glm::vec3(GetHeight(x - 1, z) - GetHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     GetHeight(x, z + 1) - GetHeight(x, z - 1))); // norz

            vertices.push_back({pos, nor, glm::vec2(0.0, 0.0)});
            i++;
        }
    }

    int indicesCount = Size * Size * 2 * 3;
    indices.reserve(indicesCount);

    i = 0;

    for (int z = 0; z < Size; z++) {
        for (int x = 0; x < Size; x++) {

            indices.push_back(GetIndex(x, z));
            indices.push_back(GetIndex(x + 1, z));
            indices.push_back(GetIndex(x + 1, z + 1));

            indices.push_back(GetIndex(x + 1, z + 1));
            indices.push_back(GetIndex(x, z + 1));
            indices.push_back(GetIndex(x, z));
        }
    }

    return true;
}

} // namespace Chimera