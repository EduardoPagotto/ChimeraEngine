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
        #ifdef WIN32
        pLoader->setModelDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
        pLoader->setImageDir("C:\\Projetos\\ChimeraEngine\\appTest\\models\\");
        #else
        pLoader->setModelDir("../../appTest/models/");
        pLoader->setImageDir("../../appTest/models/");
        #endif
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
    
    