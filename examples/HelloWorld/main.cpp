#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/VideoDevice.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    auto console = spdlog::stdout_color_st("chimera");

    spdlog::set_level(spdlog::level::debug);

    console->info("AppEmpty Iniciado");
    for (int i = 0; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/HelloWorld/etc/empty.yaml";

        console->info("Carregar arquivo:{0}", config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { console->error("Falha grave:{0}", ex); } catch (...) {
        console->error("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    console->info("AppShader finalizado com sucesso");

    return 0;
}
