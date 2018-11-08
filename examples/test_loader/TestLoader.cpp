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

        ChimeraLoaders::Scene lib("./models/piso2.xml");
        auto var = lib.target();

        Chimera::Group *pGroup = std::get<0>(var);
        Chimera::PhysicsControl *pPC = std::get<1>(var);

	} catch (const Chimera::Exception& ex) {
        console->error("TesteLoader falha grave:{0}", ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("TesteLoader falha grave:{0}", ex.what());
        return -1;
    } catch (const std::string& ex) {
        console->error("TesteLoader falha grave:{0}", ex);
        return -1;
    } catch (...) {
        console->error("TesteLoader falha Desconhecida");
        return -1;
    }

	console->info("TesteLoader finalizado com sucesso");
	return 0;
}


