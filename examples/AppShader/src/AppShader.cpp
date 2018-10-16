#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>
#include "VideoDevice.h"
#include "Game.h"
#include "FlowControl.h"
#include "ExceptionChimera.h"

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    auto console = spdlog::stdout_color_st("main");
    console->info("AppShader Iniciado");
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

	try {
		std::string config_file = "./etc/examples/AppShader/shader.yaml";
        console->info("Carregar arquivo:{0}",config_file);
        YAML::Node config = YAML::LoadFile(config_file);


		//Instancia de Video
		//Chimera::Video *video = new Chimera::OvrDevice("Teste");
		Chimera::Video *video = new Chimera::VideoDevice(800, 600, "teste");

		Game *game = new Game(video);

		Chimera::FlowControl *pControle = new Chimera::FlowControl(game);
		pControle->open();
		pControle->gameLoop();

		delete pControle;
		delete game;
		delete video;

	}
	catch (const Chimera::Exception& ex) {

		std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
		return -1;
	}

	return 0;
}


