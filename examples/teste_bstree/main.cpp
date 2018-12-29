#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include <iostream>

int main(int argn, char** argv) {

    Chimera::Logger* console = Chimera::Logger::get();
    console->setLevel(Chimera::Logger::Level::DEBUG);

    try {
        Chimera::CanvasGL* video = new Chimera::CanvasGL("TesteBSTree", 640, 480);
        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave: " + ex.getMessage());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave: %s", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { console->error("Falha grave: " + ex); } catch (...) {
        console->error("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    console->info("TesteBSTree finalizado com sucesso");

    return 0;
}
