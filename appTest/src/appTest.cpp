#include <iostream>
#include <cstdio>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

#include "Game.h"
#include "Loader.h"

//using namespace log4cxx;
//using namespace log4cxx::xml;
//using namespace log4cxx::helpers;

int main ( int argn, char** argv ) {

    // Load XML configuration file using DOMConfigurator
    log4cxx::xml::DOMConfigurator::configure ( "../../appTest/config/Log4cxxConfig.xml" );   
    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("appTest");
    
    //std::cout << "A inicializar sistema.." << std::endl;
    LOG4CXX_INFO ( logger , "Inicio app!!" );
    
    try {

        Chimera::Engine3D *engine3d = new Chimera::Engine3D();        
        Chimera::Video *video = new Chimera::Video ( 640, 480, std::string ( "Teste" ) );
        
        //std::cout << "Tela Iniciada com sucesso " <<std::endl;
        LOG4CXX_INFO ( logger , "Tela Iniciada com sucesso" );
        
        engine3d->initOpenGL(video);
        std::cout << "OpenGL iniciado com sucesso, versao: " << engine3d->getVersaoOpenGL()  <<std::endl;
        
        Chimera::SceneMng *sceneMng = new Chimera::SceneMng(engine3d);
       
        Chimera::Loader *pLoader = new Chimera::Loader(sceneMng);
        pLoader->exec("../../appTest/models/simples1.dae");
        
        Chimera::Camera *camera = new Chimera::Camera(Chimera::CameraType::AIR_CAM,"camera01","camera01");
        camera->setPosition(btVector3(-500.0, -500.0, 500.0));
        camera->setDirection(btVector3(0.0, 0.0, 0.0));
        camera->setRotation(btVector3(0.0, 0.0, 1.0));
        camera->setNear(0.01);
        camera->setFar(5000);
        camera->setFov(1.33);
        
        
        
        
        sceneMng->addChildToScene(camera);
        
        Chimera::Object *obj = new Chimera::Object("objeto01","objeto01");
        obj->setPosition(btVector3(0.0, 0.0, 0.0));
        obj->setDirection(btVector3(0.0, 0.0, 0.0));
        obj->setRotation(btVector3(0.0, 0.0, 0.0));
        sceneMng->addChildToScene(obj);
        
        Chimera::DrawBox *drawBox = new Chimera::DrawBox(5.0, 5.0, 5.0);
        obj->setDraw(drawBox);
        
        Chimera::Light *luz1 = new Chimera::Light(Chimera::LightType::POINT,0,"luz01","luz01");
        luz1->setPosition(btVector3(-50.0, -50.0, -50.0));
        luz1->setAmbient(Chimera::Color(0.2, 0.2, 0.2));
        luz1->setDiffuse(Chimera::Color(0.8, 0.8, 0.8));
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

    } catch ( const Chimera::ExceptionSDL& ex ) {

        //std::cerr << "Engine Erro Critico SDL:" << ex.what() << " codigo " << ex.getCode() << std::endl;
        //printf("Erro %s codigo %d na camada SDL", ex.what(), ex.getCode() );c
        LOG4CXX_ERROR ( logger , "Engine Erro Critico SDL:" << ex.what() );
        
    } catch ( const Chimera::Exception& ex1 ) {

        //std::cerr << "Engine Erro Critico Sistema:" << ex1.what() << " codigo " << ex1.getCode() << std::endl;
        //printf("Erro %s codigo %d", ex1.what(), ex1.getCode() );
        LOG4CXX_ERROR ( logger , "Engine Erro Critico Sistema:" << ex1.what() );

    } catch ( const std::exception& ex2 ) {

        //std::cerr << "Engine Erro Critico Generico:" << ex2.what() << std::endl;
        //printf("Erro %s", ex2.what() );
        LOG4CXX_ERROR ( logger , "Engine Erro Critico Generico:" );

    }

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
