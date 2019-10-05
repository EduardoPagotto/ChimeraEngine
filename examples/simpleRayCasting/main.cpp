#include "Game.hpp"
#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include <iostream>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        Chimera::CanvasFB* video = new Chimera::CanvasFB("RayCasting", 640, 480);
        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        SDL_Log("Falha grave: %s", ex.getMessage().c_str());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        SDL_Log("Falha grave: %s", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { SDL_Log("Falha grave: %s", ex.c_str()); } catch (...) {
        SDL_Log("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    SDL_Log("raycasting finalizado com sucesso");

    return 0;
}
