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

#include <glm\glm.hpp>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

	struct TesteZ {
		glm::vec3 pos; //4 X 3 = 12
		glm::vec3 norm;//4 X 3 = 12
		glm::vec2 tex;//4 X 2  =  8
		glm::vec4 cor;  //4 X4 =  16
	};

	TesteZ v[2];
	v[0].cor = glm::vec4(1, 1, 1, 1);
	v[0].pos = glm::vec3(2, 2, 2);
	v[0].norm = glm::vec3(3, 3, 3);
	v[0].tex = glm::vec2(4, 4);

	v[1].cor = glm::vec4(5, 5, 5, 5);
	v[1].pos = glm::vec3(6, 6, 6);
	v[1].norm = glm::vec3(7, 7, 7);
	v[1].tex = glm::vec2(8, 8);


	unsigned int sizeA = sizeof(TesteZ);
	
	//void *memoria = malloc(sizeA);
	float val[64];
	memset(&val[0], 0, 64 * 4);
	memcpy(&val[0], &v[0], 128);


    using namespace Chimera;
    
    try {

#ifdef WIN32
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\appTest\\models";
#else
        std::string dirDados = "../../appTest/models";
#endif
        
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
        pCam->setPositionRotation( btVector3(300 , 0 ,0) , btVector3(0,0,0) );
        pCam->setPerspective(true);
        pCam->init();
        sceneMng->cameraAtiva(pCam);
      
        
        Group* group1 = sceneMng->createSceneGraph();
//#ifdef WIN32
//        group1->setIdProgram( Chimera::LoadShaders ( "C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\vertex.glsl",
//                                            "C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\fragment.glsl" ));
//# else
//        group1->setIdProgram( Chimera::LoadShaders ( "/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/vertex.glsl",
//                                            "/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/fragment.glsl" ));
//# endif
         
        Transform* pTrans = new Transform(group1,"trans01");
        //pTrans->setPosition( btVector3( 0.0, 35.0, -350.0) );
        pTrans->setPosition( btVector3( 0.0, 0.0, 0.0) );
        
        sceneMng->origemDesenho((Coord*)pTrans);
        
        Texture *pTex = new Texture("Texture-teste",dirDados + "/spacebox.png");
        
        Mesh *pMesh = Mesh::createMeshParallelepiped(pTrans, "Cubo-01",glm::vec3(50,50,50),nullptr, nullptr);
        
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