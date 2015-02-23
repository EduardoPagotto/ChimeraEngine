#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>

#include "VideoDevice.h"
#include "OvrDevice.h"
#include "Game.h"
#include "DrawBox.h"
#include "DrawGrid.h"
#include "Loader.h"

#include "LoaderDae.h"

int testeCargaArquivo() {

	try {

		//Instancia de Video
		Chimera::Video *video = new Chimera::OvrDevice("Teste");
		//Chimera::Video *video = new Chimera::VideoDevice(1200, 800, "teste");

		//Carga de dados 
		Chimera::Loader *pLoader = new Chimera::Loader();
#ifdef WIN32
		pLoader->setModelDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
		pLoader->setImageDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
#else
		pLoader->setModelDir("../../appTest/models/");
		pLoader->setImageDir("../../appTest/models/");
#endif
		//Chimera::Node *pRoot = pLoader->loadDAE("cuboTex1.dae");//cuboEesfera.dae
		//Chimera::Node *pRoot = pLoader->loadDAE("cuboEesfera.dae");
		//Chimera::Node *pRoot = pLoader->loadDAE("testeMaterial.xml");
		//Chimera::Node *pRoot = pLoader->loadDAE("CuboEsferaMaterial.dae");
		Chimera::Node *pRoot = pLoader->loadDAE("piso1.dae");
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

		std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
		return -1;
	}

	return 0;
}

int testeCargaManual() {

	try {

		//Instancia de Video
		Chimera::Video *video = new Chimera::VideoDevice(1200, 800, "teste");
		//Chimera::Video *video = new Chimera::OvrDevice("Teste");
		Chimera::Node *pRoot = new Chimera::Node(Chimera::EntityKind::NODE, "cena01", "cona01");

		Chimera::CameraSpherical *pCam = new Chimera::CameraSpherical("Camera01", "Camera01");
		pCam->setNear(0.1f);
		pCam->setFar(1000.0f);
		pCam->setFov(45.0f);
		pCam->setPerspective(true);
		pCam->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
		pRoot->addChild(pCam);

		Chimera::Light *pLight = new Chimera::Light(Chimera::LightType::POINT, "Luz01", "Luz01");
		pLight->setDiffuse(Chimera::Color(1.0f, 0.5f, 0.5f, 1.0f));
		pLight->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
		pRoot->addChild(pLight);

		Chimera::Effect *pEffect = new Chimera::Effect("cor01", "cor01");
		pEffect->setDiffuse(Chimera::Color(1.0f, 1.0f, 1.0f));
		pEffect->setSpecular(Chimera::Color(1.0f, 1.0f, 1.0f));

		Chimera::DrawBox *pDraw = new Chimera::DrawBox("caixa01", "caixa01");
		pDraw->setSizeBox(btVector3(5.0f, 5.0f, 5.0f));

		Chimera::Physics *pPysics = new Chimera::Physics("rigid-body01", "rigid-body01");
		pPysics->setMass(10.0f);
		pPysics->setFriction(0.0f);
		pPysics->setRestitution(0.0f);

		Chimera::Object *pObj = new Chimera::Object("ator01", "ator01");
		pObj->setPositionRotation(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
		pObj->addChild(pDraw);
		pObj->addChild(pPysics);
		pObj->addChild(pEffect);

		pRoot->addChild(pObj);

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

		std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
		return -1;

	}

	return 0;
}

int testeNovaCarga() {
	
	ChimeraLoader::LoaderDae *pLoader = new ChimeraLoader::LoaderDae("../../appTest/models/","../../appTest/models/testeMaterial.xml");
	
	
}

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif
	//return testeCargaManual();
	//return testeCargaArquivo();
	return testeNovaCarga();
}


