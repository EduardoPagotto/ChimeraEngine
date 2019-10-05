#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/ShadersLoader.hpp"
#include <iostream>

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("AppEmpty Iniciado");

        Chimera::CanvasGL* video = new Chimera::CanvasGL("TesteBSTree", 640, 480);

        // sempre depois de instanciar o Opengl no canvas!!!
        Chimera::ShadersLoader sl;
        Chimera::Shader* pShader = sl.loadShader("Simples1", "./examples/teste_bstree/shaders/simples.vert",
                                                 "./examples/teste_bstree/shaders/simples.frag");

        Game* game = new Game(video, pShader);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        SDL_Log("Falha grave: %s", ex.getMessage().c_str());
        return -1;
    } catch (const std::exception& ex) { SDL_Log("Falha grave: %s", ex.what()); } catch (const std::string& ex) {
        SDL_Log("Falha grave: %s", ex.c_str());
    } catch (...) { SDL_Log("Falha Desconhecida"); }

    SDL_Log("TesteBSTree finalizado com sucesso");
    return 0;
}
