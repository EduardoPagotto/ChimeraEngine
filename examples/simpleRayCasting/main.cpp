#include "Game.hpp"
#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif
    auto console = spdlog::stdout_color_st("chimera");

    spdlog::set_level(spdlog::level::debug);

    try {
        Chimera::CanvasFB* video = new Chimera::CanvasFB("RayCasting", 640, 480);
        Game* game = new Game(video);

        Chimera::FlowControl* pControle = new Chimera::FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        // std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
        // std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) { console->error("Falha grave:{0}", ex); } catch (...) {
        console->error("Falha Desconhecida");
        // std::cout << "Falha Desconhecida " << std::endl;
    }

    console->info("raycasting finalizado com sucesso");

    return 0;
}
