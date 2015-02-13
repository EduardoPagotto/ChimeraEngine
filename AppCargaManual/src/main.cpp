#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>

//#include <log4cxx/logger.h>
//#include <log4cxx/xml/domconfigurator.h>

#include "VideoDevice.h"
#include "OvrDevice.h"
#include "Game.h"
#include "DrawBox.h"
#include "DrawGrid.h"
#include "Loader.h"

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

	try {

		//Instancia de Video
		//Chimera::Video *video = new Chimera::OvrDevice("Teste");
		Chimera::Video *video = new Chimera::VideoDevice(800, 640, "teste");
		
		//Carga de dados 
		Chimera::Loader *pLoader = new Chimera::Loader();
		pLoader->setModelDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
		pLoader->setImageDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
		//Chimera::Node *pRoot = pLoader->loadDAE("cuboTex1.dae");//cuboEesfera.dae
		Chimera::Node *pRoot = pLoader->loadDAE("cuboEesfera.dae");
		//Chimera::Node *pRoot = pLoader->loadDAE("testeMaterial.dae");
		//Chimera::Node *pRoot = pLoader->loadDAE("zoltan.dae");

		delete pLoader;
		pLoader = nullptr;

		Chimera::SceneMng *sceneMng = new Chimera::SceneMng(pRoot);

		Game *game = new Game(video, sceneMng);
		game->open();
		
		game->gameLoop();
		
		delete game;
		delete sceneMng;
		delete pRoot;
		delete video;
	}
	catch (const Chimera::Exception& ex) {

	}

	return 0;
}



//#ifndef WIN32
//int main ( int argn, char** argv ) {
//#else
//int _tmain(int argc, _TCHAR* argv[]) {
//#endif
//
//    using namespace Chimera;
//
//    // Load XML configuration file using DOMConfigurator
//    //log4cxx::xml::DOMConfigurator::configure ( "../../appTest/config/Log4cxxConfig.xml" );
//    //log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger ( "appTest" );
//    //LOG4CXX_INFO ( logger , "Inicio app!!" );
//
//    try {
//
//        //Instancia de Video
//        //Chimera::Video *video = new Chimera::VideoDevice( 800, 600, std::string ( "Teste" ) );
//		Chimera::Video *video = new Chimera::OvrDevice("Teste");
//		//LOG4CXX_INFO ( logger , "Tela Iniciada com sucesso" );
//
//		Node *pRoot = new Node ( EntityKind::NODE, "cena01","cona01" );
//
//		CameraSpherical *pCam = new CameraSpherical("Camera01","Camera01");
//		pCam->setNear(0.1f);
//		pCam->setFar(1000.0f);
//		pCam->setFov(45.0f);
//		pCam->setPerspective(true);
//		pCam->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
//		pRoot->addChild(pCam);
//
//		Light *pLight = new Light(LightType::POINT, "Luz01","Luz01");
//		pLight->setDiffuse(Color( 1.0f, 0.5f, 0.5f, 1.0f));
//		pLight->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
//		pRoot->addChild(pLight);
//
//		Effect *pEffect = new Effect("cor01","cor01");
//		pEffect->setDiffuse(Color(1.0f, 1.0f, 1.0f));
//		pEffect->setSpecular(Color(1.0f, 1.0f, 1.0f));
//
//		DrawBox *pDraw = new DrawBox("caixa01","caixa01");
//		pDraw->setSizeBox(btVector3(5.0f,5.0f,5.0f));
//
//		Physics *pPysics = new Physics("rigid-body01","rigid-body01");
//		pPysics->setMass(10.0f);
//		pPysics->setFriction(0.0f);
//		pPysics->setRestitution(0.0f);
//
//		Object *pObj = new Object("ator01","ator01");
//		pObj->setPositionRotation(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
//		pObj->addChild(pDraw);
//		pObj->addChild(pPysics);
//		pObj->addChild(pEffect);
//
//		pRoot->addChild(pObj);
//
//        SceneMng *sceneMng = new SceneMng ( pRoot );
//
//        Game *game = new Game ( video, sceneMng );
//        game->open();
//        //LOG4CXX_INFO ( logger , "Game inicializado com sucesso" );
//
//        game->gameLoop();
//        //LOG4CXX_INFO ( logger , "Engine encerrado com sucesso" );
//
//        delete game;
//        delete sceneMng;
//        delete pRoot;
//        delete video;
//
//    } catch ( const Exception& ex ) {
//		std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
//		return -1;
//        //LOG4CXX_ERROR ( logger , "Engine Erro Critico Sistema:" << ex.what() );
//    }
//
//    return 0;
//}
