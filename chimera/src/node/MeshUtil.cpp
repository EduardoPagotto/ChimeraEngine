#include "chimera/node/MeshUtil.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

void debugDados(Mesh* pMesh) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Debug Mess Nome: %s", pMesh->getName().c_str());
    pMesh->meshData.debugDados();
}

Mesh* createMeshParallelepiped(Node* _pParent, const std::string& _name, const glm::vec3& _size, Material* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj(_size.x / 2.0f, _size.y / 2.0f, _size.z / 2.0f);

    // VertexList
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, -sizeObj.z));

    // VertexIndex
    int vertexIndexArray[] = {01, 02, 03, 07, 06, 05, 04, 05, 01, 05, 06, 02, 06, 07, 03, 00, 03, 07,
                              00, 01, 03, 04, 07, 05, 00, 04, 01, 01, 05, 02, 02, 06, 03, 04, 00, 07};

    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->meshData.vertexIndex.insert(pMesh->meshData.vertexIndex.end(), &vertexIndexArray[0],
                                       &vertexIndexArray[vertexIndexArraySize]);

    // NormalList
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->meshData.normalList.push_back(glm::vec3(1, -0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(-0, -1, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(-1, 0, -0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 1, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->meshData.normalList.push_back(glm::vec3(1, 0, -0));
    pMesh->meshData.normalList.push_back(glm::vec3(-0, -1, -0));
    pMesh->meshData.normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 1, 0));

    // NormalIndex
    int normalIndexArray[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,  4,  4,  5,  5,  5,
                              6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->meshData.normalIndex.insert(pMesh->meshData.normalIndex.end(), &normalIndexArray[0],
                                       &normalIndexArray[normalIndexArraySize]);

    Material* pMatFinal = _pMaterial;

    if (pMatFinal == nullptr)
        pMatFinal = new Material("Mat-" + _name + "-" + std::to_string(pMesh->getSerial() + 1));
    else {

        if (pMatFinal->hasTexture() == true) {

            // TextureList
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.667));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.333));

            int texturaIndexArray[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
                                       18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

            unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
            pMesh->meshData.textureIndex.insert(pMesh->meshData.textureIndex.end(), &texturaIndexArray[0],
                                                &texturaIndexArray[texturaIndexArraySize]);
        }
    }

    pMesh->setMaterial(pMatFinal);

    return pMesh;
}

Mesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);
    Material* pMatFinal = _pMaterial;
    if (pMatFinal == nullptr)
        pMatFinal = new Material("Mat-" + _name + "-" + std::to_string(pMesh->getSerial() + 1));

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}

Mesh* createMeshParallelepiped2(Node* _pParent, const std::string& _name, const glm::vec3& _size,
                                Material* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);

    glm::vec3 sizeObj(_size.x / 2.0f, _size.y / 2.0f, _size.z / 2.0f);

    // VertexList
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, -sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, -sizeObj.y, sizeObj.z));
    pMesh->meshData.vertexList.push_back(glm::vec3(-sizeObj.x, sizeObj.y, sizeObj.z));

    // VertexIndex
    int vertexIndexArray[] = {00, 01, 03, 04, 07, 05, 00, 04, 01, 01, 05, 02, 02, 06, 03, 04, 00, 07,
                              01, 02, 03, 07, 06, 05, 04, 05, 01, 05, 06, 02, 06, 07, 03, 00, 03, 07};

    unsigned vertexIndexArraySize = sizeof(vertexIndexArray) / sizeof(int);
    pMesh->meshData.vertexIndex.insert(pMesh->meshData.vertexIndex.end(), &vertexIndexArray[0],
                                       &vertexIndexArray[vertexIndexArraySize]);

    // NormalList
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->meshData.normalList.push_back(glm::vec3(1, 0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, -1, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 1, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, -1));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 0, 1));
    pMesh->meshData.normalList.push_back(glm::vec3(1, 0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, -1, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(-1, 0, 0));
    pMesh->meshData.normalList.push_back(glm::vec3(0, 1, 0));

    // NormalIndex
    int normalIndexArray[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,  4,  4,  5,  5,  5,
                              6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11};

    unsigned normalIndexArraySize = sizeof(normalIndexArray) / sizeof(int);
    pMesh->meshData.normalIndex.insert(pMesh->meshData.normalIndex.end(), &normalIndexArray[0],
                                       &normalIndexArray[normalIndexArraySize]);

    Material* pMatFinal = _pMaterial;

    if (pMatFinal == nullptr)
        pMatFinal = new Material("Mat-" + _name + "-" + std::to_string(pMesh->getSerial() + 1));
    else {

        if (pMatFinal->hasTexture() == true) {
            // TextureList
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 1.000));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.250, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.000, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(1.000, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.666));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.500, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.333));
            pMesh->meshData.textureList.push_back(glm::vec2(0.750, 0.666));

            int texturaIndexArray[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
                                       18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

            unsigned texturaIndexArraySize = sizeof(texturaIndexArray) / sizeof(int);
            pMesh->meshData.textureIndex.insert(pMesh->meshData.textureIndex.end(), &texturaIndexArray[0],
                                                &texturaIndexArray[texturaIndexArraySize]);
        }
    }

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}
} // namespace Chimera
