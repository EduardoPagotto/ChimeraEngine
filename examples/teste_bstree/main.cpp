#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/core/ShadersLoader.hpp"
#include <iostream>

int main(int argn, char** argv) {

    Chimera::Logger* console = Chimera::Logger::get();
    console->setLevel(Chimera::Logger::Level::DEBUG);

    try {
        // Chimera::ShadersLoader* sl = new Chimera::ShadersLoader();
        // Chimera::Shader* pShader =
        //     sl->loadShader("Simples1", "./examples/teste_bstree/simples.vert",
        //     "./examples/teste_bstree/simples.frag");
        // delete sl;
        // sl = nullptr;

        Chimera::CanvasGL* video = new Chimera::CanvasGL("TesteBSTree", 640, 480);
        Game* game = new Game(video, nullptr);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave: " + ex.getMessage());
        return -1;
    } catch (const std::exception& ex) { console->error("Falha grave: %s", ex.what()); } catch (const std::string& ex) {
        console->error("Falha grave: " + ex);
    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("TesteBSTree finalizado com sucesso");
    return 0;
}
