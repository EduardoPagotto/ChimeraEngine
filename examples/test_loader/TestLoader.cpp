#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include "ExceptionChimera.hpp"
#include <spdlog/spdlog.h>

#include "PhysicsScene.hpp"
#include "VisualScene.hpp"

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    auto console = spdlog::stdout_color_st("chimera");
    console->info("Teste Loader Iniciado");

    spdlog::set_level(spdlog::level::debug);
    for (int i = 0; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {

        // Carrega Referencias a texturas mesh, cans, light, trans, em nodes encadeados em
        // arvore
        Chimera::Group* group1 = new Chimera::Group(nullptr, "none");
        ChimeraLoaders::VisualScene libV("./models/piso2.xml", group1);
        libV.target();

        // Carrega dados fisicos do ambiente, solidos e os vincula aos mesh por singleton
        // de lista de nodes
        Chimera::PhysicsControl* pPC = new Chimera::PhysicsControl();
        ChimeraLoaders::PhysicsScene libP("./models/piso2.xml", pPC);
        libP.target();

    } catch (const Chimera::ExceptionBase& ex) {
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
