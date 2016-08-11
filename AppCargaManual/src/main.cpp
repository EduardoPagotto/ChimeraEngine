#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>

#include "VideoDevice.h"

#ifdef OVR_SET_TO_USE
#include "OvrDevice.h"
#endif

#include "Game.h"
#include "LoaderDae.h"
#include "FlowControl.h"
#include "ParticleEmitter.h"
#include "Transform.h"
#include "HUD.h"
#include "Shader.h"

int testeCargaArquivo() {

    try {

#ifdef WIN32
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\appTest\\models";
#else
        std::string dirDados = "../../appTest/models";
#endif

        //Instancia de Video
        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        Chimera::Video *video = new Chimera::VideoDevice ( 800, 600, "teste" );

        //Carga de dados
		Chimera::LoaderDae *pLoader = new Chimera::LoaderDae ( dirDados, dirDados, "piso2.xml" );
        //Chimera::Node *pRoot = pLoader->loadFile("cuboTex1.dae");//cuboEesfera.dae
        //Chimera::Node *pRoot = pLoader->loadFile("cuboEesfera.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("testeMaterial.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("CuboEsferaMaterial.dae");
        //Chimera::Node *pRoot = pLoader->loadFile ( "piso2.xml" ); // piso2Teste.xml
        //Chimera::Node *pRoot = pLoader->loadFile("piso2Teste.xml");
        //Chimera::Node *pRoot = pLoader->loadFile("espacoTesteZ1.xml");
        //Chimera::Node *pRoot = pLoader->loadFile("zoltan.dae");

        Chimera::SceneMng *sceneMng = new Chimera::SceneMng ( video );
        sceneMng->setReader(pLoader);
        Chimera::Group* group1 = sceneMng->createSceneGraph();
        
//http://www.kickjs.org/example/shader_editor/shader_editor.html template para usar
//http://www.lighthouse3d.com/tutorials/glsl-tutorial/attribute-variables/ como carregar
  #ifdef WIN32
  		group1->shader.load("C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\vertex.glsl",
  		                    "C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\fragment.glsl");
  # else
  		group1->shader.load("/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/vertex.glsl",
  		                    "/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/fragment.glsl");
  # endif

		//Adiciona um Emissor de Particula 
		//Chimera::Transform* posParticle = new Chimera::Transform((Chimera::Node*)group1, "posicaoParticle");
		//posParticle->setTranslate(btVector3(-5.0, 5.0, 4.0));
        Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter ((Chimera::Node*)group1, "testeZ1" );
		pParticleEmitter->setPosSource(glm::vec3(-5.0, 5.0, 4.0));
		pParticleEmitter->setMaxSeed(100);
        std::string pathFilePaticle = dirDados + std::string ( "/" ) +std::string ( "Particle.png" );
        pParticleEmitter->loadImage ( pathFilePaticle.c_str() ) ;

		//Adiciona um HUD ao Grapho
		Chimera::HUD *pHUD = new Chimera::HUD(group1, "HUD-Default");
	#ifdef WIN32
		Chimera::Font *pFont = new Chimera::Font ( "C:\\Projetos\\ChimeraEngine\\fonts\\FreeSans.ttf", 18 );
	#else
		Chimera::Font *pFont = new Chimera::Font ( "../../fonts/FreeSans.ttf", 18 );
	#endif
		pHUD->addFont ( pFont );
		SDL_Rect area = { 30, 30, 600, 20 };
		pHUD->addSquare ( area, Chimera::Color ( 1.0f, 1.0f, 1.0f, 0.2f ) );

        delete pLoader;
        pLoader = nullptr;

        Game *game = new Game ( sceneMng );

        Chimera::FlowControl *pControle = new Chimera::FlowControl ( game );
        pControle->open();
        pControle->gameLoop();

        delete pControle;
        delete game;
        delete sceneMng;
        delete video;
        
    } catch ( const Chimera::Exception& ex ) {

        std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    }

    return 0;
}

int testeCargaManual() {

    //try {

    //	//Instancia de Video
    //	Chimera::Video *video = new Chimera::VideoDevice(1200, 800, "teste");
    //	//Chimera::Video *video = new Chimera::OvrDevice("Teste");

    //	Chimera::Node *pRoot = new Chimera::Node(Chimera::EntityKind::NODE, "cena01", "cona01");

    //	Chimera::CameraSpherical *pCam = new Chimera::CameraSpherical("Camera01", "Camera01");
    //	pCam->setNear(0.1f);
    //	pCam->setFar(1000.0f);
    //	pCam->setFov(45.0f);
    //	pCam->setPerspective(true);
    //	pCam->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
    //	pRoot->addChild(pCam);

    //	Chimera::Light *pLight = new Chimera::Light(Chimera::LightType::POINT, "Luz01", "Luz01");
    //	pLight->setDiffuse(Chimera::Color(1.0f, 0.5f, 0.5f, 1.0f));
    //	pLight->setPositionRotation(btVector3(15.0f, 15.0f, 15.0f), btVector3(45.0f, 45.0f, 45.0f));
    //	pRoot->addChild(pLight);

    //	Chimera::Effect *pEffect = new Chimera::Effect("cor01", "cor01");
    //	pEffect->setDiffuse(Chimera::Color(1.0f, 1.0f, 1.0f));
    //	pEffect->setSpecular(Chimera::Color(1.0f, 1.0f, 1.0f));

    //	Chimera::Material *pMaterial = new Chimera::Material("Mat01", "Mat01");
    //	pMaterial->addChild(pEffect);

    //	Chimera::DrawBox *pDraw = new Chimera::DrawBox("caixa01", "caixa01");
    //	pDraw->setSizeBox(btVector3(5.0f, 5.0f, 5.0f));
    //	pDraw->addChild(pMaterial);

    //	Chimera::Physics *pPysics = new Chimera::Physics("rigid-body01", "rigid-body01");
    //	pPysics->setMass(10.0f);
    //	pPysics->setFriction(0.0f);
    //	pPysics->setRestitution(0.0f);

    //	Chimera::Object *pObj = new Chimera::Object("ator01", "ator01");
    //	pObj->setPositionRotation(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
    //	pObj->addChild(pDraw);
    //	pObj->addChild(pPysics);


    //	pRoot->addChild(pObj);

    //	Chimera::SceneMng *sceneMng = new Chimera::SceneMng(pRoot, video);

    //	Game *game = new Game(video, sceneMng);
    //	game->open();

    //	game->gameLoop();

    //	delete game;
    //	delete sceneMng;
    //	delete pRoot;
    //	delete video;

    //}
    //catch (const Chimera::Exception& ex) {

    //	std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
    //	return -1;

    //}

    return 0;
}

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif
    //return testeCargaManual();
    return testeCargaArquivo();
}


