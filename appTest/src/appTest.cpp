#include <iostream>
#include <cstdio>
#include "Game.h"

int main ( int argn, char** argv ) {

    using namespace Chimera;

    std::cout << "A inicializar sistema.." << std::endl;
  
    try {

        Engine3D *engine3d = new Engine3D();        
        Video *video = new Video ( 640, 480, std::string ( "Teste" ) );
        std::cout << "Tela Iniciada com sucesso " <<std::endl;
        
        engine3d->initOpenGL(video);
        std::cout << "OpenGL iniciado com sucesso, versao: " << engine3d->getVersaoOpenGL()  <<std::endl;
        
        SceneMng *sceneMng = new SceneMng();
        
        
        
        
        
        
        
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

    } catch ( const ExceptionSDL& ex ) {

        std::cerr << "Engine Erro Critico SDL:" << ex.what() << " codigo " << ex.getCode() << std::endl;
        //printf("Erro %s codigo %d na camada SDL", ex.what(), ex.getCode() );c

    } catch ( const Exception& ex1 ) {

        std::cerr << "Engine Erro Critico Sistema:" << ex1.what() << " codigo " << ex1.getCode() << std::endl;
        //printf("Erro %s codigo %d", ex1.what(), ex1.getCode() );

    } catch ( const std::exception& ex2 ) {

        std::cerr << "Engine Erro Critico Generico:" << ex2.what() << std::endl;
        //printf("Erro %s", ex2.what() );

    }

    
   

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
