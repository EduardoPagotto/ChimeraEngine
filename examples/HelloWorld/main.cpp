#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    Chimera::Logger* log = Chimera::Logger::get();
    log->info("AppEmpty Iniciado");

    for (int i = 0; i < argn; i++) {
        log->info("Parametros %d: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/HelloWorld/etc/empty.yaml";

        log->info("Carregar arquivo: %s", config_file.c_str());
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];
        Chimera::CanvasGL* video =
            new Chimera::CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());

        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        log->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        log->info("Falha grave: %s", ex.getMessage());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        log->info("Falha grave: %s", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { log->info("Falha grave: %s", ex); } catch (...) {
        log->info("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    log->info("AppShader finalizado com sucesso");
    return 0;
}
