#include "Game.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/utils.hpp"

#include <iostream>

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("AppEmpty Iniciado");

        Chimera::CanvasGL* video = new Chimera::CanvasGL("TesteBSTree", 1400, 900);
        Chimera::Shader* pShader = new Chimera::Shader("MeshNoMat", "./chimera/shaders/MeshNoMat.vert", "./chimera/shaders/MeshNoMat.frag");

        Game* game = new Game(video, pShader);

        Chimera::IO::FlowControl* pControle = new Chimera::IO::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        delete pControle;
        delete game;
        delete video;

        SDL_Log("TesteBSTree finalizado com sucesso");
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
