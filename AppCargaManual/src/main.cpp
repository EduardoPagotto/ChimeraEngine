#include <iostream>
#include <cstdio>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

#include "Game.h"
#include "DrawBox.h"
//#include "Loader.h"

int main ( int argn, char** argv ) {

    using namespace Chimera;

    // Load XML configuration file using DOMConfigurator
    log4cxx::xml::DOMConfigurator::configure ( "../../appTest/config/Log4cxxConfig.xml" );
    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger ( "appTest" );
    LOG4CXX_INFO ( logger , "Inicio app!!" );

    try {

        //Instancia de Video
        Chimera::Video *video = new Chimera::Video ( 800, 600, std::string ( "Teste" ) );
        LOG4CXX_INFO ( logger , "Tela Iniciada com sucesso" );

		Node *pRoot = new Node ( EntityKind::NODE, "cena01","cona01" );
		
		CameraSpherical *pCam = new CameraSpherical("Camera01","Camera01");
		pCam->setNear(0.1f);
		pCam->setFar(1000.0f);
		pCam->setFov(45.0f);
		pCam->setPerspective(true);
		pCam->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
		pRoot->addChild(pCam);
		
		Light *pLight = new Light(LightType::POINT, "Luz01","Luz01");
		pLight->setDiffuse(Color( 1.0f, 0.5f, 0.5f, 1.0f));
		pLight->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
		pRoot->addChild(pLight);
		
		Effect *pEffect = new Effect("cor01","cor01");
		pEffect->setDiffuse(Color(1.0f, 1.0f, 1.0f));
		pEffect->setSpecular(Color(1.0f, 1.0f, 1.0f));
		
		DrawBox *pDraw = new DrawBox("caixa01","caixa01");
		pDraw->setSizeBox(btVector3(1.0f,1.0f,1.0f));
		
		Physics *pPysics = new Physics("rigid-body01","rigid-body01");
		pPysics->setMass(10.0f);
		pPysics->setFriction(0.0f);
		pPysics->setRestitution(0.0f);
		
		Object *pObj = new Object("ator01","ator01");
		pObj->setPositionRotation(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
		pObj->addChild(pDraw);
		pObj->addChild(pPysics);
		pObj->addChild(pEffect);

		Object *pObj2 = nullptr;
		pObj->clone((Node**)&pObj2 );
		pObj2->setPositionRotation(btVector3(5.0f, 5.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
		pRoot->addChild(pObj2);
		
		
		
		
		
		pRoot->addChild(pObj);
		
        SceneMng *sceneMng = new SceneMng ( pRoot );

        Game *game = new Game ( video, sceneMng );
        game->open();
        LOG4CXX_INFO ( logger , "Game inicializado com sucesso" );

        game->gameLoop();
        LOG4CXX_INFO ( logger , "Engine encerrado com sucesso" );

        delete game;
        delete sceneMng;
        delete pRoot;
        delete video;


    } catch ( const Exception& ex ) {

        LOG4CXX_ERROR ( logger , "Engine Erro Critico Sistema:" << ex.what() );

    }

    return 0;
}
