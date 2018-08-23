#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>
#include <VideoDevice.h>
#include "Game.h"
#include <FlowControl.h>
#include <ExceptionChimera.h>

//
#include <spdlog/spdlog.h>
//#include <spdlog/sinks/stdout_color_sinks.h>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

    try {

        auto console = spdlog::stdout_color_st("console");
        console->info("Welcome to spdlog!") ;
        console->info("An info message example {}..", 1);

        for (int i=0 ; i < argn; i++) {
            console->info("Valor {0}: {1}",i ,argv[i]) ;
        }

        //Instancia de Video
        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        Chimera::Video *video = new Chimera::VideoDevice (800, 600, "teste");

        Game *game = new Game(video);

		Chimera::FlowControl *pControle = new Chimera::FlowControl( game );
		pControle->open();
		pControle->gameLoop();

        delete pControle;
        delete game;
        delete video;

    } catch ( const Chimera::Exception& ex ) {

        std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    }

    return 0;
}


