#include "appTest.h"

int main ( int argn, char** argv ) {

    using namespace Chimera;

    std::cout << "A inicializar sistema.." << std::endl;

    Video *video = nullptr;
    SceneMng *sceneMng = nullptr;
    Game *game = nullptr;
   
    try {

        video = new Video ( 640, 480, std::string ( "Teste" ) );
        sceneMng = new SceneMng(video);
        sceneMng->open();
       
        game = new Game(video,sceneMng);
        

        game->init();
        std::cout << "Engine inicializado com sucesso" << std::endl;

        game->gameLoop();
        std::cout << "Engine encerrado com sucesso" << std::endl;

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

    delete video;
    delete sceneMng;
    delete game;

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
