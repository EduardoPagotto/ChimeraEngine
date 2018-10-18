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

    auto console = spdlog::stdout_color_st("chimera");

    spdlog::set_level(spdlog::level::debug);
    
    console->info("AppShader Iniciado");

    spdlog::set_level(spdlog::level::debug);

    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

	try {
		std::string config_file = "./examples/AppShader/etc/shader.yaml";
        console->info("Carregar arquivo:{}",config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        std::string nome = screen["name"].as<std::string>();

        YAML::Node canvas = screen["canvas"];
        int w = canvas["w"].as<int>();
        int h = canvas["h"].as<int>();

		YAML::Node shader = config["shader"];
		std::string vertexFile = shader["vertex"].as<std::string>();
		std::string fragmentFile = shader["fragment"].as<std::string>();

        console->info("Iniciar Tela: {0}, w: {1:03d}, h: {2:03d}", nome, w, h);
		console->info("Shader: Vertex: {}", vertexFile);
		console->info("Shader: Fragment: {}", fragmentFile);
		
		//Instancia de Video
		//Chimera::Video *video = new Chimera::OvrDevice("Teste");
		Chimera::Video *video = new Chimera::VideoDevice(w, h, nome);

		Game *game = new Game(vertexFile, fragmentFile, video);

		Chimera::FlowControl *pControle = new Chimera::FlowControl(game);
		pControle->open();
		pControle->gameLoop();

		console->info("Loop de Game encerrado!!!!");

		delete pControle;
		delete game;
		delete video;

	} catch (const Chimera::Exception& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) {
        console->error("Falha Desconhecida");
    }

	console->info("AppShader finalizado com sucesso");
	return 0;
}


