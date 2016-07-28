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
        Group* group1 = sceneMng->createSceneGraph();
        
        Transform* pTrans = new Transform(group1,"trans01");
        pTrans->setPosition( btVector3(0.0, 0.0, 0.0) );
        
        Texture *pTex = new Texture("Texture-teste",dirDados + "/spacebox.png");
        
        Mesh *pMesh = Mesh::createMeshParallelepiped(pTrans, "Cubo-01",glm::vec3(50,50,50),pTex, nullptr);
        
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