
#include "chimera/core/Exception.hpp"
#include "chimera/loader/PhysicsScene.hpp"
#include "chimera/loader/VisualScene.hpp"
#include <cstdio>

#include <SDL2/SDL.h>

int main(int argn, char** argv) {

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("Teste Loader Iniciado");
    // spdlog::set_level(spdlog::level::debug);
    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %i: %s", i, argv[i]);
    }

    try {

        // Carrega Referencias a texturas mesh, cans, light, trans, em nodes encadeados em
        // arvore
        Chimera::Group* group1 = new Chimera::Group(nullptr, "none");
        ChimeraLoaders::VisualScene libV("./models/caixa.dae", group1);
        // ChimeraLoaders::VisualScene libV("./models/piso2.xml", group1);
        libV.target();

        // Carrega dados fisicos do ambiente, solidos e os vincula aos mesh por singleton
        // de lista de nodes
        Chimera::PhysicsControl* pPC = new Chimera::PhysicsControl();
        ChimeraLoaders::PhysicsScene libP("./models/piso2.xml", pPC);
        libP.target();

    } catch (const Chimera::Exception& ex) {
        SDL_Log("TesteLoader falha grave: %s", ex.getMessage().c_str());
        return -1;
    } catch (const std::exception& ex) {
        SDL_Log("TesteLoader falha grave: %s", ex.what());
        return -1;
    } catch (const std::string& ex) {
        SDL_Log("TesteLoader falha grave: %s", ex.c_str());
        return -1;
    } catch (...) {
        SDL_Log("TesteLoader falha Desconhecida");
        return -1;
    }

    SDL_Log("TesteLoader finalizado com sucesso");
    return 0;
}
