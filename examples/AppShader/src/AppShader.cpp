#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include "ExceptionChimera.h"
#include "FlowControl.h"
#include "Game.h"
#include "VideoDevice.h"
#include <iostream>

#include "ShadersLoader.h"

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    auto console = spdlog::stdout_color_st("chimera");

    spdlog::set_level(spdlog::level::debug);

    console->info("AppShader Iniciado");

    spdlog::set_level(spdlog::level::debug);

    for (int i = 0; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/AppShader/etc/shader.yaml";
        console->info("Carregar arquivo:{}", config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        YAML::Node shader = config["shader"];

        Chimera::Video* video =
            new Chimera::VideoDevice(canvas["w"].as<int>(), canvas["h"].as<int>(),
                                     screen["name"].as<std::string>());

        ChimeraLoaders::ShadersLoader* pSL = new ChimeraLoaders::ShadersLoader();
        Chimera::Shader* pShader =
            pSL->loadShader("default", shader["vertex"].as<std::string>(),
                            shader["fragment"].as<std::string>());

        Game* game = new Game(pShader, video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete pShader;
        delete video;

    } catch (const Chimera::ExceptionBase& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("AppShader finalizado com sucesso");
    return 0;
}
