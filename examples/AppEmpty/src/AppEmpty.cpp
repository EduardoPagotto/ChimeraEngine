#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>
//#include <VideoDevice.h>
#include <HmdDevice.h>
#include "Game.h"
#include <FlowControl.h>
#include <ExceptionChimera.h>

#include <spdlog/spdlog.h>
#include "yaml-cpp/yaml.h"

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

    auto console = spdlog::stdout_color_st("main");
    console->info("Empty Iniciado");
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
        std::string config_file = "./etc/examples/AppEmpty/empty.yaml";

        console->info("Carregar arquivo:{0}",config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        std::string nome = screen["name"].as<std::string>();

        YAML::Node canvas = screen["canvas"];
        int w = canvas["w"].as<int>();
        int h = canvas["h"].as<int>();

        console->info("Iniciar Tela {0}, w:{1}, h:{2}", nome, w, h);

        //Instancia de Video
        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        //Chimera::Video *video = new Chimera::VideoDevice (w, h, nome);
        Chimera::Video *video = new Chimera::HmdDevice("teste");

        Game *game = new Game(video);

		Chimera::FlowControl *pControle = new Chimera::FlowControl( game );
		pControle->open();
		pControle->gameLoop();

        console->warn("Encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch ( const Chimera::Exception& ex ) {
        console->error("Falha grave:{0}", ex.getMessage());
        //std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
        //std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) {
        console->error("Falha Desconhecida");
        //std::cout << "Falha Desconhecida " << std::endl;
    }

    return 0;
}


