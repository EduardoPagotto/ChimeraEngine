#include "chimera/core/partition/Wavefront.hpp"
#include "chimera/core/collada/Collada.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include <fstream>

namespace Chimera {

glm::vec4 tokensToVec4(std::vector<std::string>& textData) {
    std::vector<float> arrayFloat;
    for (int indice = 1; indice < textData.size(); indice++) {
        if (textData[indice].size() > 0)
            arrayFloat.push_back(std::stod(textData[indice]));
    }

    if (arrayFloat.size() < 4)
        return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], 1.0f);

    return glm::vec4(arrayFloat[0], arrayFloat[1], arrayFloat[2], arrayFloat[3]);
}

glm::vec3 tokensToVec3(unsigned start, unsigned total, std::vector<std::string>& textData) {

    std::vector<float> arrayFloat;
    for (int indice = start; arrayFloat.size() < total; indice++) {
        if (textData[indice].size() > 0)
            arrayFloat.push_back(std::stod(textData[indice]));
    }

    return glm::vec3(arrayFloat[0], arrayFloat[1], arrayFloat[2]);
}

glm::vec2 tokensToVec2(unsigned start, unsigned total, std::vector<std::string>& textData) {

    std::vector<float> arrayFloat;
    for (int indice = start; arrayFloat.size() < total; indice++) {
        if (textData[indice].size() > 0)
            arrayFloat.push_back(std::stod(textData[indice]));
    }

    return glm::vec2(arrayFloat[0], arrayFloat[1]);
}

void wavefrontMtlLoad(const std::string& path, Material* material) {
    std::ifstream file(path);

    if (!file.is_open())
        throw std::string("ERROR: could not open file: " + path);

    std::string lineBuffer;
    while (std::getline(file, lineBuffer)) {
        std::string first = lineBuffer.substr(0, 1);
        if (first == "#")
            continue;

        std::vector<std::string> textData;
        textToStringArray(lineBuffer, textData, ' ');

        if (textData.size() == 0)
            continue;

        if (textData[0] == "Ka") {
            material->setAmbient(tokensToVec4(textData));
        } else if (textData[0] == "Kd") {
            material->setDiffuse(tokensToVec4(textData));
        } else if (textData[0] == "Ks") {
            material->setSpecular(tokensToVec4(textData));
        } else if (textData[0] == "map_Kd") {
            material->addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::loadFromFile(textData[1], textData[1], TexParam()));
        } else if (textData[0] == "sharpness") {
            material->setShine(std::stod(textData[1]));
        }
    }
}

void wavefrontObjLoad(const std::string& path, Mesh* mesh, std::string& fileMath) {
    std::ifstream file(path);

    if (!file.is_open())
        throw std::string("ERROR: could not open file: " + path);

    std::vector<int> indicesComp;

    std::string lineBuffer;
    while (std::getline(file, lineBuffer)) {
        std::string first = lineBuffer.substr(0, 1);
        if (first == "#")
            continue;

        std::vector<std::string> textData;
        textToStringArray(lineBuffer, textData, ' ');

        if (textData.size() == 0)
            continue;

        if (textData[0] == "mtllib")
            fileMath = textData[1];
        else if (textData[0] == "v")
            mesh->point.push_back(tokensToVec3(1, 3, textData));
        else if (textData[0] == "vt")
            mesh->uv.push_back(tokensToVec2(1, 2, textData));
        else if (textData[0] == "vn")
            mesh->normal.push_back(tokensToVec3(1, 3, textData));
        else if (textData[0] == "f") {
            int face = 0;
            for (int indice = 1; indice < textData.size(); indice++) {
                if (textData[indice].size() > 0) {
                    std::vector<std::string> ss;
                    if (face > 8)
                        break;

                    textToStringArray(textData[indice], ss, '/');
                    for (std::string cc : ss) {
                        if (cc.size() > 0) {
                            indicesComp.push_back(std::stod(cc) - 1);
                            face++;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::string> semantics;
    semantics.push_back("VERTEX"); // 0

    if (mesh->uv.size() > 0)
        semantics.push_back("TEXCOORD"); // 1

    if (mesh->normal.size() > 0)
        semantics.push_back("NORMAL"); // 2

    std::vector<uint32_t> iPoint;
    std::vector<uint32_t> iNormal;
    std::vector<uint32_t> iUv;

    for (uint32_t l_contador = 0; l_contador < indicesComp.size(); l_contador++) {

        uint32_t index = l_contador % semantics.size();
        const std::string& semantic = semantics[index];

        if (semantic == "VERTEX")
            iPoint.push_back(indicesComp[l_contador]);
        else if (semantic == "NORMAL")
            iNormal.push_back(indicesComp[l_contador]);
        else if (semantic == "TEXCOORD")
            iUv.push_back(indicesComp[l_contador]);
    }

    for (uint32_t i = 0; i < iPoint.size(); i += 3)
        mesh->iPoint.push_back({iPoint[i], iPoint[i + 1], iPoint[i + 2]});

    for (uint32_t i = 0; i < iNormal.size(); i += 3)
        mesh->iNormal.push_back({iNormal[i], iNormal[i + 1], iNormal[i + 2]});

    for (uint32_t i = 0; i < iUv.size(); i += 3)
        mesh->iUv.push_back({iUv[i], iUv[i + 1], iUv[i + 2]});

    mesh->serialized = false;

    file.close();
}
} // namespace Chimera