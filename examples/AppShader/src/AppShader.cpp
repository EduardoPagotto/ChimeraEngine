#include <cstdio>

#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/loader/ShadersLoader.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    auto console = Chimera::Logger::get();
    console->info("AppShader Iniciado");

    for (int i = 0; i < argn; i++) {
        console->info("Parametros %i: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/AppShader/etc/shader.yaml";
        console->info("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        YAML::Node shader = config["shader"];

        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        ChimeraLoaders::ShadersLoader* pSL = new ChimeraLoaders::ShadersLoader();
        Chimera::Shader* pShader =
            pSL->loadShader("default", shader["vertex"].as<std::string>(), shader["fragment"].as<std::string>());

        Game* game = new Game(pShader, video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete pShader;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave: " + ex.getMessage());
        return -1;
    } catch (const std::exception& ex) { console->error("Falha grave: %s", ex.what()); } catch (const std::string& ex) {
        console->error("Falha grave: " + ex);
    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("AppShader finalizado com sucesso");
    return 0;
}
