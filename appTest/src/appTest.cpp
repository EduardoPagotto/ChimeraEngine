#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>
#include "VideoDevice.h"
#include "Game.h"
#include "FlowControl.h"
#include "ExceptionChimera.h"

#include "Transform.h"
#include "Shader.h"

#include "CameraSpherical.h"

#include "Light.h"

#include "MeshUtil.h"

#include <glm/glm.hpp>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

#ifdef WIN32
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\models\\";
		std::string dirBase = "C:\\Projetos\\ChimeraEngine\\shader\\";
		std::string dirFontes = "C:\\Projetos\\ChimeraEngine\\fonts\\";
#else
        std::string dirDados = "/home/locutus/Projetos/ChimeraEngine/models/";
		std::string dirBase = "/home/locutus/Projetos/ChimeraEngine/shader/";
		std::string dirFontes = "/home/locutus/Projetos/ChimeraEngine/fonts/";
#endif

    using namespace Chimera;

    try {

        //Instancia de Video
        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        Video *video = new VideoDevice (640, 480, "teste");

        SceneMng *sceneMng = new SceneMng ( video );
        //sceneMng->setReader(pLoader);

        CameraSpherical *pCam = new CameraSpherical("Observador-01");
        pCam->setDistanciaMaxima(1000.0);
        pCam->setDistanciaMinima(0.5);
        pCam->setFar(10000.0);
        pCam->setNear(0.1);
        pCam->setFov(45.0);
        pCam->setPositionRotation( glm::vec3(300 , 0 ,0) , glm::vec3(0,0,0) );
        pCam->setPerspective(true);
        pCam->init();
        sceneMng->cameraAtiva(pCam);

        Group* group1 = sceneMng->createSceneGraph();

		Shader *shader =  Singleton<Shader>::getRefSingleton();
		shader->load("default", dirBase + "vertex.glsl", dirBase + "fragment.glsl");


        Transform* pTrans = new Transform(group1,"trans01");
        pTrans->setPosition( glm::vec3( 0.0, 0.0, 0.0) );

        Light* pLight = new Light(group1,"Luz1");
        pLight->setDiffuse(Color::WHITE);
        pLight->setPositionRotation(glm::vec3(80,100,150), glm::vec3(0,0,0));
        //pLight->init();

        sceneMng->origemDesenho((Coord*)pTrans);

        //Texture *pTex = new Texture("Texture-teste",dirDados + "/spacebox.png");
        //Texture *pTex = new Texture("Texture-teste",dirDados + "/image1.jpg");
		Material *pMat1 = new Material("Mat1");
		pMat1->setAmbient( Color( 1.0f, 0.5f, 0.31f ) );
        pMat1->setDiffuse( Color( 1.0f, 0.5f, 0.31f ) );
        pMat1->setSpecular( Color( 0.5f, 0.5f, 0.5f ) );
        pMat1->setShine(32.0f);
		//pMat1->setTexDiffuse(pTex);

        Mesh *pMesh = Chimera::createMeshParallelepiped2(pTrans, "Cubo-01",glm::vec3(50,50,50), pMat1);

		Transform* pTrans2 = new Transform(group1, "trans02");
		pTrans2->setPosition(glm::vec3(150.0, 0.0, 0.0));

		Material *pMat2 = new Material("Mat2");
        pMat2->setAmbient( Color( 0.5f, 0.5f, 0.31f ) );
        pMat2->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
        pMat2->setSpecular( Color( 0.5f, 0.5f, 0.5f ) );
        pMat2->setShine(32.0f);
		//pMat2->setTexDiffuse(pTex);

		Mesh *pMesh2 = Chimera::createMeshParallelepiped2(pTrans2, "Cubo-02", glm::vec3(20, 20, 20), pMat2);


        Game *game = new Game(sceneMng);

        FlowControl *pControle = new FlowControl( game );
        pControle->open();
        pControle->gameLoop();

        delete pControle;
        delete game;
        delete video;

    } catch ( const Exception& ex ) {

        std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    }

    return 0;
}
