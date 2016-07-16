#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>
#include "VideoDevice.h"
#include "Node.h"
#include "Game.h"
#include "FlowControl.h"
#include "ExceptionChimera.h"

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

	try {

		//Instancia de Video
		//Chimera::Video *video = new Chimera::OvrDevice("Teste");
		Chimera::Video *video = new Chimera::VideoDevice(800, 600, "teste");
		Chimera::SceneMng *sceneMng = new Chimera::SceneMng(video);
		Chimera::Group* group1 = sceneMng->createSceneGraph();

		Game *game = new Game(sceneMng);

		Chimera::FlowControl *pControle = new Chimera::FlowControl(video, game);
		pControle->open();
		pControle->gameLoop();

		delete pControle;
		delete game;
		delete video;

	}
	catch (const Chimera::Exception& ex) {

		std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
		return -1;
	}
}


