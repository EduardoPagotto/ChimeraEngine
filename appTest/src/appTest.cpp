#include <iostream>
#include <cstdio>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

#include "Game.h"
#include "Loader.h"

int main ( int argn, char** argv ) {

    // Load XML configuration file using DOMConfigurator
    log4cxx::xml::DOMConfigurator::configure ( "../../appTest/config/Log4cxxConfig.xml" );   
    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("appTest");
    
    LOG4CXX_INFO ( logger , "Inicio app!!" );
    
    try {    
        
        Chimera::Engine3D *engine3d = new Chimera::Engine3D();        
        Chimera::Video *video = new Chimera::Video ( 640, 480, std::string ( "Teste" ) );
        
        LOG4CXX_INFO ( logger , "Tela Iniciada com sucesso" );
        
        engine3d->initOpenGL(video);
        std::string l_msg =  "OpenGL iniciado com sucesso, versao: " + engine3d->getVersaoOpenGL();
        LOG4CXX_INFO ( logger , l_msg );
              
        Chimera::Loader *pLoader = new Chimera::Loader();
        pLoader->setModelDir("../../appTest/models/");
        pLoader->setImageDir("../../appTest/models/");
        Chimera::Node *pRoot = pLoader->loadDAE("cuboTex1.dae");
        
        Chimera::SceneMng *sceneMng = new Chimera::SceneMng(pRoot);
                        
        Game *game = new Game(engine3d,sceneMng);
        game->open();
        std::cout << "Game inicializado com sucesso" << std::endl;

        game->gameLoop();
        std::cout << "Engine encerrado com sucesso" << std::endl;

        delete video;
        delete engine3d;
        delete sceneMng;
        delete game;
        
        //SDL_Delay(2000);
 
    } catch ( const Chimera::Exception& ex ) {

        LOG4CXX_ERROR ( logger , "Engine Erro Critico Sistema:" << ex.what() );

    } catch ( const std::exception& ex2 ) {

        LOG4CXX_ERROR ( logger , "Engine Erro Critico Generico:" );
    }

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
