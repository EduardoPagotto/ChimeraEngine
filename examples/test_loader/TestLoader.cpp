
#include "chimera/core/Exception.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/loader/PhysicsScene.hpp"
#include "chimera/loader/VisualScene.hpp"
#include <cstdio>

int main(int argn, char** argv) {

    Chimera::Logger* console = Chimera::Logger::get();
    console->info("Teste Loader Iniciado");
    // spdlog::set_level(spdlog::level::debug);
    for (int i = 0; i < argn; i++) {
        console->info("Parametros %i: %s", i, argv[i]);
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
        console->error("TesteLoader falha grave: " + ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("TesteLoader falha grave: %s", ex.what());
        return -1;
    } catch (const std::string& ex) {
        console->error("TesteLoader falha grave: " + ex);
        return -1;
    } catch (...) {
        console->error("TesteLoader falha Desconhecida");
        return -1;
    }

    console->info("TesteLoader finalizado com sucesso");
    return 0;
}
