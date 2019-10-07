#include <cstdio>

#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/ShadersLoader.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("AppShader Iniciado");

    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %i: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./samples/shader/shader.yaml";
        SDL_Log("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        YAML::Node shader = config["shader"];

        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        Chimera::ShadersLoader* pSL = new Chimera::ShadersLoader();
        Chimera::Shader* pShader =
            pSL->loadShader("default", shader["vertex"].as<std::string>(), shader["fragment"].as<std::string>());

        Game* game = new Game(pShader, video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete pShader;
        delete video;

    } catch (const Chimera::Exception& ex) {
        SDL_Log("Falha grave: %s", ex.getMessage().c_str());
        return -1;
    } catch (const std::exception& ex) { SDL_Log("Falha grave: %s", ex.what()); } catch (const std::string& ex) {
        SDL_Log("Falha grave: %s", ex.c_str());
    } catch (...) { SDL_Log("Falha Desconhecida"); }

    SDL_Log("AppShader finalizado com sucesso");
    return 0;
}
