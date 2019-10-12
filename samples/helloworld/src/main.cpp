#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"

#include "Game.hpp"

#include <iostream>
#include <yaml-cpp/yaml.h>

int main(int argn, char** argv) {

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("AppEmpty Iniciado");

    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %d: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./samples/helloworld/empty.yaml";

        SDL_Log("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // Exception Chimera
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what());
    } catch (const std::exception& ex) {
        // Exception generica
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what());
    } catch (const std::string& ex) {
        // Exception string
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.c_str());
    } catch (...) {
        // desconhecida
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro Desconhecida");
    }

    return -1;
}
