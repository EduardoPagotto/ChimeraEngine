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
        
        SceneMng *sceneMng = new SceneMng(engine3d);
       
        Camera *camera = new Camera("camera01",CameraType::AIR_CAM);
        camera->setPosition(btVector3(-500.0, -500.0, 500.0));
        camera->setDirection(btVector3(0.0, 0.0, 0.0));
        camera->setRotation(btVector3(0.0, 0.0, 1.0));
        camera->set(0.01, 5000, 1.33);
        sceneMng->addChildToScene(camera);
        
        Object *obj = new Object("objeto01");
        obj->setPosition(btVector3(0.0, 0.0, 0.0));
        obj->setDirection(btVector3(0.0, 0.0, 0.0));
        obj->setRotation(btVector3(0.0, 0.0, 0.0));
        sceneMng->addChildToScene(obj);
        
        DrawBox *drawBox = new DrawBox(5.0, 5.0, 5.0);
        obj->setDraw(drawBox);
        
        Light *luz1 = new Light(LightType::POINT,0,"luz01");
        luz1->setPosition(btVector3(-50.0, -50.0, -50.0));
        luz1->setAmbient(Color(0.2, 0.2, 0.2));
        luz1->setDiffuse(Color(0.8, 0.8, 0.8));
        sceneMng->addChildToScene(luz1);
        
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
