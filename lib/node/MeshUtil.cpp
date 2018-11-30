#include "MeshUtil.hpp"

#include <spdlog/spdlog.h>

namespace Chimera {

void debugDados(Mesh* pMesh) {

    std::shared_ptr<spdlog::logger> log = spdlog::get("chimera");
    log->debug("Debug Mess Nome: {}", pMesh->getName());

    int linha = 0;
    log->debug("Vertex Indice ----------({0:03d})", pMesh->vertexIndex.size());
    for (unsigned int indice = 0; indice < pMesh->vertexIndex.size(); indice += 3) {
        log->debug("Linha:{0:02d} : p:{1:02d} ({2:02d}; {3:02d}; {4:02d})", linha, indice,
                   pMesh->vertexIndex[indice], pMesh->vertexIndex[indice + 1],
                   pMesh->vertexIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    log->debug("Vertex Lista ---------({0:03d})", pMesh->vertexList.size());
    for (unsigned int indice = 0; indice < pMesh->vertexList.size(); indice++) {
        log->debug("Linha: {0:02d} : p:{1:02d} ({2:05.3f}; {3:05.3f}; {4:05.3f})", linha,
                   indice, pMesh->vertexList[indice].x, pMesh->vertexList[indice].y,
                   pMesh->vertexList[indice].z);
        linha++;
    }

    linha = 0;
    log->debug("Normal Indice ----------({0:03d})", pMesh->normalIndex.size());
    for (unsigned int indice = 0; indice < pMesh->normalIndex.size(); indice += 3) {
        log->debug("Linha: {0:02d} : p:{1:02d} ({2:02d}; {3:02d}; {4:02d})", linha,
                   indice, pMesh->normalIndex[indice], pMesh->normalIndex[indice + 1],
                   pMesh->normalIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    log->debug("Normal Lista ---------({0:03d})", pMesh->normalList.size());
    for (unsigned int indice = 0; indice < pMesh->normalList.size(); indice++) {
        log->debug("Linha: {0:02d} : p:{1:02d} ({2:05.3f}; {3:05.3f}; {4:05.3f})", linha,
                   indice, pMesh->normalList[indice].x, pMesh->normalList[indice].y,
                   pMesh->normalList[indice].z);
        linha++;
    }

    linha = 0;
    log->debug("Texture Indice ----------({0:03d})", pMesh->textureIndex.size());
    for (unsigned int indice = 0; indice < pMesh->textureIndex.size(); indice += 3) {
        log->debug("Linha: {0:02d} : p: {1:02d} ({2:02d}; {3:02d}; {4:02d})", linha,
                   indice, pMesh->textureIndex[indice], pMesh->textureIndex[indice + 1],
                   pMesh->textureIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    log->debug("Texture Lista ---------({0:03d})", pMesh->textureList.size());
    for (unsigned int indice = 0; indice < pMesh->textureList.size(); indice++) {
        log->debug("Linha: {0:02d} : p: {1:02d} ({2:05.3f}; {3:05.3f})", linha, indice,
                   pMesh->textureList[indice].x, pMesh->textureList[indice].y);
        linha++;
    }
}

Mesh* createMeshParallelepiped(Node* _pParent, const std::string& _name,
                               const glm::vec3& _size, Material* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj(_size.x / 2.0f, _size.y / 2.0f, _size.z / 2.0f);

    // VertexList
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, -sizeObj.z));

    // VertexIndex
    int vertexIndexArray[] = {01, 02, 03, 07, 06, 05, 04, 05, 01, 05, 06, 02,
                              06, 07, 03, 00, 03, 07, 00, 01, 03, 04, 07, 05,
                              00, 04, 01, 01, 05, 02, 02, 06, 03, 04, 00, 07};

    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->vertexIndex.insert(pMesh->vertexIndex.end(), &vertexIndexArray[0],
                              &vertexIndexArray[vertexIndexArraySize]);

    // NormalList
    pMesh->normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->normalList.push_back(glm::vec3(1, -0, 0));
    pMesh->normalList.push_back(glm::vec3(-0, -1, 0));
    pMesh->normalList.push_back(glm::vec3(-1, 0, -0));
    pMesh->normalList.push_back(glm::vec3(0, 1, 0));
    pMesh->normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->normalList.push_back(glm::vec3(1, 0, -0));
    pMesh->normalList.push_back(glm::vec3(-0, -1, -0));
    pMesh->normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->normalList.push_back(glm::vec3(0, 1, 0));

    // NormalIndex
    int normalIndexArray[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,  4,  4,  5,  5,  5,
                              6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->normalIndex.insert(pMesh->normalIndex.end(), &normalIndexArray[0],
                              &normalIndexArray[normalIndexArraySize]);

    Material* pMatFinal = _pMaterial;

    if (pMatFinal == nullptr)
        pMatFinal =
            new Material("Mat-" + _name + "-" + std::to_string(pMesh->getSerial() + 1));
    else {

        if (pMatFinal->hasTexture() == true) {

            // TextureList
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.250, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.250, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.333));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.333));

            int texturaIndexArray[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                                       12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                       24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

            unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
            pMesh->textureIndex.insert(pMesh->textureIndex.end(), &texturaIndexArray[0],
                                       &texturaIndexArray[texturaIndexArraySize]);
        }
    }

    pMesh->setMaterial(pMatFinal);

    return pMesh;
}

Mesh* createMeshParallelepiped2(Node* _pParent, const std::string& _name,
                                const glm::vec3& _size, Material* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj(_size.x / 2.0f, _size.y / 2.0f, _size.z / 2.0f);

    // VertexList
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, sizeObj.z));

    // VertexIndex
    int vertexIndexArray[] = {00, 01, 03, 04, 07, 05, 00, 04, 01, 01, 05, 02,
                              02, 06, 03, 04, 00, 07, 01, 02, 03, 07, 06, 05,
                              04, 05, 01, 05, 06, 02, 06, 07, 03, 00, 03, 07};

    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->vertexIndex.insert(pMesh->vertexIndex.end(), &vertexIndexArray[0],
                              &vertexIndexArray[vertexIndexArraySize]);

    // NormalList
    pMesh->normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->normalList.push_back(glm::vec3(1, 0, 0));
    pMesh->normalList.push_back(glm::vec3(0, -1, 0));
    pMesh->normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->normalList.push_back(glm::vec3(0, 1, 0));
    pMesh->normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->normalList.push_back(glm::vec3(1, 0, 0));
    pMesh->normalList.push_back(glm::vec3(0, -1, 0));
    pMesh->normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->normalList.push_back(glm::vec3(0, 1, 0));

    // NormalIndex
    int normalIndexArray[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,  4,  4,  5,  5,  5,
                              6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->normalIndex.insert(pMesh->normalIndex.end(), &normalIndexArray[0],
                              &normalIndexArray[normalIndexArraySize]);

    Material* pMatFinal = _pMaterial;

    if (pMatFinal == nullptr)
        pMatFinal =
            new Material("Mat-" + _name + "-" + std::to_string(pMesh->getSerial() + 1));
    else {

        if (pMatFinal->hasTexture() == true) {
            // TextureList
            pMesh->textureList.push_back(glm::vec2(0.750, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.750, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.333));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->textureList.push_back(glm::vec2(1.000, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->textureList.push_back(glm::vec2(0.750, 0.666));

            int texturaIndexArray[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                                       12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                       24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

            unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
            pMesh->textureIndex.insert(pMesh->textureIndex.end(), &texturaIndexArray[0],
                                       &texturaIndexArray[texturaIndexArraySize]);
        }
    }

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}
} // namespace Chimera
