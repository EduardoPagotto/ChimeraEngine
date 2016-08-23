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
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\models";
		std::string dirBase = "C:\\Projetos\\ChimeraEngine\\shader\\";
		std::string dirFontes = "C:\\Projetos\\ChimeraEngine\\fonts\\";
#else
        std::string dirDados = "../../models";
		std::string dirBase = "../../shader/";
		std::string dirFontes = "../../fonts/";
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
		group1->shader.load("default", dirBase + "vertex.glsl", dirBase + "fragment.glsl");

		//Adiciona um Emissor de Particula
		//Chimera::Transform* posParticle = new Chimera::Transform((Chimera::Node*)group1, "posicaoParticle");
		//posParticle->setTranslate(btVector3(-5.0, 5.0, 4.0));
        //Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter ((Chimera::Node*)group1, "testeZ1", 10000 );
		//pParticleEmitter->setPosSource(glm::vec3(-5.0, 5.0, 4.0));
		//pParticleEmitter->setMaxSeed(100);
        //std::string pathFilePaticle = dirDados + std::string ( "/" ) +std::string ( "Particle.png" );
        //pParticleEmitter->loadImage ( pathFilePaticle.c_str() ) ;

		//Novo Emissor GLSL
		Chimera::Group *gParticle = new Chimera::Group( (Chimera::Node*)sceneMng->getRoot(), "ParticleGroup" );
		gParticle->shader.load("default", dirBase + "ParticleVertexShader.glsl", dirBase + "ParticleFragmentShader.glsl");

		Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter((Chimera::Node*)gParticle, "testeZ1", 10000);
		Chimera::Material *pMatParticleEmiter = new Chimera::Material("MatParticleEmitter");
		Chimera::Texture *pTexParticleEmitter = new Chimera::Texture("TexParticleEmmiter", dirDados + std::string("/") + std::string("Particle2.png"));
		pMatParticleEmiter->setTexDiffuse(pTexParticleEmitter);
		pParticleEmitter->getState()->setMaterial(pMatParticleEmiter);

        Chimera::Group *gHud = new Chimera::Group( (Chimera::Node*)sceneMng->getRoot(), "HUD-Group");
		gHud->shader.load("default", dirBase +  "HudVertexShader.glsl", dirBase + "HudFragmentShader.glsl");

		//Adiciona um HUD ao Grapho
		Chimera::HUD *pHUD = new Chimera::HUD(gHud, "HUD-Default");
		Chimera::Font *pFont = new Chimera::Font ( dirFontes + "FreeSans.ttf", 18 );
		pHUD->addFont ( pFont );

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


