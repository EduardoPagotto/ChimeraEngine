#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <spdlog/spdlog.h>
#include <ExceptionChimera.h>

#include <Scene.h>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

    auto console = spdlog::stdout_color_st("chimera");
    console->info("Teste Loader Iniciado");

    spdlog::set_level(spdlog::level::debug);
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {

        ChimeraLoaders::Scene lib("./models/piso2_old.xml");
        auto var = lib.target();


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


