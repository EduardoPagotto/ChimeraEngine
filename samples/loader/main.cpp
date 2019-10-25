
#include "chimera/core/Exception.hpp"
#include "chimera/loader/VisualScene.hpp"
#include "chimera/physic_loader/PhysicsScene.hpp"
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

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // Exception Chimera
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what());
    } catch (const std::exception& ex) {
        // Exception generica
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what());
    } catch (const std::string& ex) {
        // Exception string
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.c_str());
    } catch (...) {
        // desconhecida
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro Desconhecida");
    }

    return -1;
}
