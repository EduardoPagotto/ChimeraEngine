#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/utils.hpp"

#include <iostream>

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        Chimera::CanvasGL* video = new Chimera::CanvasGL("simples", 640, 480);

        Chimera::Shader* pShader =
            new Chimera::Shader("Simples1", Chimera::shadeLoadProg("Simples1", "./samples/bsptree/shaders/simples.vert",
                                                                   "./samples/bsptree/shaders/simples.frag"));

        Game* game = new Game(video, pShader);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        delete pControle;
        delete game;
        delete video;

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fali 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
