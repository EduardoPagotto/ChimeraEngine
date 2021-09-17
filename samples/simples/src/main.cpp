#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include <iostream>

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        Chimera::Core::CanvasGL* video = new Chimera::Core::CanvasGL("simples", 640, 480);

        Chimera::Shader* pShader =
            new Chimera::Shader("Simples1", "./samples/simples/shaders/simples.vert", "./samples/simples/shaders/simples.frag");

        Game* game = new Game(video, pShader);

        Chimera::Core::FlowControl* pControle = new Chimera::Core::FlowControl(game);
        pControle->run();

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
