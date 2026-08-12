#include "Loader.hpp"
#include <SDL3/SDL.h>
#include <iostream>
// #include <SDL3/SDL_init.h>
// #include <fastgltf/core.hpp>
// #include <fastgltf/glm_element_traits.hpp>
// #include <fastgltf/tools.hpp>
// #include <fastgltf/types.hpp>
// #include <filesystem>
// #include <glm/glm.hpp>

int main() {

    int result = SDL_APP_SUCCESS;

    MeshData mesh;

    try {

        // Habilita todas as mensagens em modo Debug
        SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

        ce::Loader loader("./assets/seahawk/Seahawk.gltf");

        // loader.testMat();
        loader.testMesh();

        // loadModel("./assets/models/teste/cubo.gltf", &mesh);

        // Dados da imagem estão prontos para serem carregados para a GPU
        // (Ex: usando stbi_load_from_memory ou vkCmdCopyBufferToImage)

        // std::cout << "Tot Indices: " << mesh.indices.size() << '\n';
        // for (const auto& indice : mesh.indices) {
        //     std::cout << "idx:" << indice << '\n';
        // }

        // std::cout << "Tot Vertex: " << mesh.vertices.size() << '\n';
        // for (const auto& vertice : mesh.vertices) {

        //     std::cout << "Position: " << vertice.position.x << ", " << vertice.position.y << ", " <<
        //     vertice.position.z; std::cout << "\t | Normal: " << vertice.normal.x << ", " << vertice.normal.y
        //     << ", "
        //     << vertice.normal.z; std::cout << "\t | UV: " << vertice.uv.x << ", " << vertice.uv.y << '\n';
        // }
        // std::cout << "Texture URI: " << mesh.textureName << '\n';

        std::cout << "teste OK" << '\n';

    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << '\n';
        result = SDL_APP_FAILURE;
    }

    return result;
}
