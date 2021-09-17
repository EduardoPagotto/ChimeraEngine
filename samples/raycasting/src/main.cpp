#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/FlowControl.hpp"
#include <iostream>

#include "Game.hpp"

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        Chimera::CanvasFB* video = new Chimera::CanvasFB("RayCasting", 640, 480);
        Game* game = new Game(video);

        Chimera::Core::FlowControl* pControle = new Chimera::Core::FlowControl(game);
        pControle->run();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

        SDL_Log("raycasting finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        SDL_Log("Falha grave: %s", ex.what());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
    } catch (const std::exception& ex) {
        SDL_Log("Falha grave: %s", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { SDL_Log("Falha grave: %s", ex.c_str()); } catch (...) {
        SDL_Log("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    return -1;
}
