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

#include "MeshUtil.h"

#include <glm/glm.hpp>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

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
        pCam->setPositionRotation( glm::vec3(300 , 0 ,0) , glm::vec3(0,0,0) );
        pCam->setPerspective(true);
        pCam->init();
        sceneMng->cameraAtiva(pCam);
      
        Group* group1 = sceneMng->createSceneGraph();
//  #ifdef WIN32
//         group1->shader.load( "C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\vertex.glsl",
//                              "C:\\Projetos\\ChimeraEngine\\AppCargaManual\\shader\\fragment.glsl");
//  #else
//         group1->shader.load( "/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/vertex.glsl",
//                              "/home/locutus/Projetos/ChimeraEngine/AppCargaManual/shader/fragment.glsl");
//  #endif
         
        Transform* pTrans = new Transform(group1,"trans01");
        pTrans->setPosition( glm::vec3( 0.0, 0.0, 0.0) );
        
        Light* pLight = new Light(group1,"Luz1"); 
        pLight->setDiffuse(Color::WHITE);
        pLight->setPositionRotation(glm::vec3(100,100,100), glm::vec3(0,0,0));
        //pLight->init();
        
        sceneMng->origemDesenho((Coord*)pTrans);
        
        //Texture *pTex = new Texture("Texture-teste",dirDados + "/spacebox.png");
        Texture *pTex = new Texture("Texture-teste",dirDados + "/image1.jpg");
        
        Mesh *pMesh = Chimera::createMeshParallelepiped2(pTrans, "Cubo-01",glm::vec3(50,50,50),pTex, nullptr);
        
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