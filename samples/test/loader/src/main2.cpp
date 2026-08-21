#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string textureName; // Caminho ou identificador da textura
};

std::vector<MeshData> loadSceneMeshes(const std::filesystem::path& gltfFilePath) {
    // 1. Create the data buffer using the modern static constructor
    auto expectedBuffer = fastgltf::GltfDataBuffer::FromPath(gltfFilePath);

    // 2. Always validate that the file read succeeded
    if (expectedBuffer.error() != fastgltf::Error::None) {
        std::cerr << "Failed to read file buffer into memory.\n";
        // return;
    }

    fastgltf::Parser parser;
    constexpr auto options = fastgltf::Options::LoadExternalBuffers;

    // 3. Parse the asset using the buffer (.get() unpacks the expected value)
    auto expectedAsset = parser.loadGltf(expectedBuffer.get(), gltfFilePath.parent_path(), options);
    if (expectedAsset.error() != fastgltf::Error::None) {
        std::cerr << "Failed to parse glTF structure.\n";
        // return;
    }

    fastgltf::Asset asset = std::move(expectedAsset.get());

    std::vector<MeshData> sceneMeshes;

    // 3. Itera sobre a cena padrão
    const auto& scene = asset.scenes[asset.defaultScene.value_or(0)];
    for (const auto& nodeIdx : scene.nodeIndices) {
        const auto& node = asset.nodes[nodeIdx];
        if (!node.meshIndex.has_value())
            continue;

        const auto& mesh = asset.meshes[node.meshIndex.value()];

        // 4. Itera sobre os primitives da mesh
        for (const auto& primitive : mesh.primitives) {
            MeshData meshData;

            // --- INDEX BUFFER ---
            if (primitive.indicesAccessor.has_value()) {
                const auto& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
                meshData.indices.resize(indexAccessor.count);

                // Copia os dados do índice usando os métodos de delegação do fastgltf
                fastgltf::copyFromAccessor<uint32_t>(asset, indexAccessor, meshData.indices.data());
            }

            // --- VERTEX BUFFER (Posição, Normal e UV) ---
            const auto& posAttr = primitive.findAttribute("POSITION");
            const auto& normAttr = primitive.findAttribute("NORMAL");
            const auto& uvAttr = primitive.findAttribute("TEXCOORD_0");

            if (posAttr != primitive.attributes.end()) {
                const auto& posAccessor = asset.accessors[posAttr->accessorIndex];
                meshData.vertices.resize(posAccessor.count);

                fastgltf::iterateAccessor<glm::vec3>(asset, posAccessor, [&](glm::vec3 pos) { positions.push_back(pos); });

                // Iterando e preenchendo os dados do vértice usando tipos do GLM
                for (size_t i = 0; i < posAccessor.count; ++i) {
                    fastgltf::iterateAccessor<glm::vec3>(asset, posAccessor, [&](glm::vec3 pos) { positions.push_back(pos); });
                }
            }

            if (normAttr != primitive.attributes.end()) {
                const auto& normAccessor = asset->accessors[normAttr->second];
                for (size_t i = 0; i < normAccessor.count; ++i) {
                    fastgltf::getAccessorData<glm::vec3>(*asset, normAccessor, i, meshData.vertices[i].normal);
                }
            }

            if (uvAttr != primitive.attributes.end()) {
                const auto& uvAccessor = asset->accessors[uvAttr->second];
                for (size_t i = 0; i < uvAccessor.count; ++i) {
                    fastgltf::getAccessorData<glm::vec2>(*asset, uvAccessor, i, meshData.vertices[i].uv);
                }
            }

            // --- TEXTURAS ---
            // Recupera o material associado à primitive
            if (primitive.materialIndex.has_value()) {
                const auto& material = asset->materials[primitive.materialIndex.value()];

                // Checa se existe uma textura base color (Albedo)
                if (material.pbrData.baseColorTexture.has_value()) {
                    const auto& textureInfo = material.pbrData.baseColorTexture.value();
                    const auto& texture = asset->textures[textureInfo.textureIndex];

                    if (texture.imageIndex.has_value()) {
                        const auto& image = asset->images[texture.imageIndex.value()];

                        // Caminho da imagem em disco ou arquivo embutido
                        if (std::holds_alternative<fastgltf::sources::URI>(image.data)) {
                            meshData.textureName = std::get<fastgltf::sources::URI>(image.data).uri.c_str();
                        }
                    }
                }
            }

            sceneMeshes.push_back(std::move(meshData));
        }
    }

    return sceneMeshes;
}

// void loadModel(const std::filesystem::path& filePath, MeshData* pMesh) {
//     // 1. Create the data buffer using the modern static constructor
//     auto expectedBuffer = fastgltf::GltfDataBuffer::FromPath(filePath);

//     // 2. Always validate that the file read succeeded
//     if (expectedBuffer.error() != fastgltf::Error::None) {
//         std::cerr << "Failed to read file buffer into memory.\n";
//         return;
//     }

//     fastgltf::Parser parser;
//     constexpr auto options = fastgltf::Options::LoadExternalBuffers;

//     // 3. Parse the asset using the buffer (.get() unpacks the expected value)
//     auto expectedAsset = parser.loadGltf(expectedBuffer.get(), filePath.parent_path(), options);
//     if (expectedAsset.error() != fastgltf::Error::None) {
//         std::cerr << "Failed to parse glTF structure.\n";
//         return;
//     }

//     fastgltf::Asset asset = std::move(expectedAsset.get());

//     for (const auto& mesh : asset.meshes) {
//         for (const auto& primitive : mesh.primitives) {

//             // --- INDEX BUFFER ---
//             if (primitive.indicesAccessor.has_value()) {
//                 auto& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
//                 pMesh->indices.resize(indexAccessor.count);
//                 // Utiliza o fastgltf::iterateAccessor para copiar para o vetor
//                 fastgltf::iterateAccessorWithIndex<uint32_t>(asset, indexAccessor,
//                                                              [&](uint32_t idx, size_t size) { pMesh->indices[size] = idx; });

//                 // (Aqui você enviaria 'indices' para um Element Array Buffer da sua API)
//             }

//             // --- VERTEX BUFFER (Posição e UVs) ---
//             const auto* posAttr = primitive.findAttribute("POSITION");
//             const auto* uvAttr = primitive.findAttribute("TEXCOORD_0");
//             const auto* normalAttr = primitive.findAttribute("NORMAL");

//             if (posAttr != primitive.attributes.end()) {
//                 auto& posAccessor = asset.accessors[posAttr->accessorIndex];
//                 pMesh->vertices.resize(posAccessor.count);

//                 // Carrega as posições
//                 fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, posAccessor, [&](fastgltf::math::fvec3 pos, size_t
//                 isize) {
//                     pMesh->vertices[isize].position[0] = pos.x();
//                     pMesh->vertices[isize].position[1] = pos.y();
//                     pMesh->vertices[isize].position[2] = pos.z();
//                 });
//             }

//             if (normalAttr != primitive.attributes.end()) {
//                 auto& normalAccessor = asset.accessors[normalAttr->accessorIndex];
//                 fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor,
//                                                                           [&](fastgltf::math::fvec3 nor, size_t isize) {
//                                                                               pMesh->vertices[isize].normal[0] = nor.x();
//                                                                               pMesh->vertices[isize].normal[1] = nor.y();
//                                                                               pMesh->vertices[isize].normal[2] = nor.z();
//                                                                           });
//             }

//             if (uvAttr != primitive.attributes.end()) {
//                 auto& uvAccessor = asset.accessors[uvAttr->accessorIndex];

//                 // Carrega as UVs
//                 fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, uvAccessor, [&](fastgltf::math::fvec2 uvd, size_t isize)
//                 {
//                     pMesh->vertices[isize].uv[0] = uvd.x();
//                     pMesh->vertices[isize].uv[1] = uvd.y();
//                 });
//             }

//             // // (Aqui você enviaria 'vertices' para o seu Vertex Buffer Object da sua API)

//             // --- TEXTURAS ---
//             if (primitive.materialIndex.has_value()) {
//                 const auto& material = asset.materials[primitive.materialIndex.value()];
//                 // Verifica se há textura baseColor
//                 if (material.pbrData.baseColorTexture.has_value()) {

//                     const auto& textureInfo = material.pbrData.baseColorTexture.value();
//                     auto& texture = asset.textures[textureInfo.textureIndex];
//                     auto& image = asset.images[texture.imageIndex.value()];

//                     // Dados da imagem estão prontos para serem carregados para a GPU
//                     // (Ex: usando stbi_load_from_memory ou vkCmdCopyBufferToImage)
//                 }
//             }
//         }
//     }
// }

int main() {

    MeshData mesh;

    loadModel("./assets/models/teste/cubo.gltf", &mesh);

    std::cout << "Tot Indices: " << mesh.indices.size() << '\n';
    for (const auto& indice : mesh.indices) {
        std::cout << "idx:" << indice << '\n';
    }

    std::cout << "Tot Vertex: " << mesh.vertices.size() << '\n';
    for (const auto& vertice : mesh.vertices) {

        std::cout << "Position: " << vertice.position.x() << ", " << vertice.position.y() << ", " << vertice.position.z();
        std::cout << "\t | Normal: " << vertice.normal.x() << ", " << vertice.normal.y() << ", " << vertice.normal.z();
        std::cout << "\t | UV: " << vertice.uv.x() << ", " << vertice.uv.y() << '\n';
    }

    std::cout << "teste OK" << '\n';
    return 0;
}
