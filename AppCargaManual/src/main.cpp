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
#include "Singleton.h"
#include "ExceptionChimera.h"

int testeCargaArquivo() {

    try {
#ifdef WIN32
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\models\\";
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

		Chimera::Shader *shader =  Chimera::Singleton<Chimera::Shader>::getRefSingleton();
		shader->load("mesh-default", dirBase + "vertex.glsl", dirBase + "fragment.glsl");
		shader->load("particle-default", dirBase + "ParticleVertexShader.glsl", dirBase + "ParticleFragmentShader.glsl");
		shader->load("hud-default", dirBase +  "HudVertexShader.glsl", dirBase + "HudFragmentShader.glsl");

		//ShadowMap
		shader->load("simpleDepthShader", dirBase + "shadow_mapping_depth.vs", dirBase + "shadow_mapping_depth.frag");
		shader->load("debugDepthQuad", dirBase + "debug_quad.vs", dirBase + "debug_quad_depth.frag");

        Chimera::SceneMng *sceneMng = new Chimera::SceneMng ( video );
        sceneMng->setReader(pLoader);
        Chimera::Group* group1 = sceneMng->createSceneGraph();
		group1->setShaderName("mesh-default");
		group1->setShadowMap(new Chimera::ShadowMapVisitor("shadow1",2048,2048));

		//Novo Emissor GLSL
		Chimera::Group *gParticle = new Chimera::Group( (Chimera::Node*)sceneMng->getRoot(), "ParticleGroup" );
		gParticle->setShaderName("particle-default");

		//Adiciona um Emissor de Particula
		Chimera::Transform* posParticle = new Chimera::Transform(gParticle, "posicaoParticle");
		posParticle->setPosition(glm::vec3(-5.0, 5.0, 4.0));
		Chimera::ParticleEmitter* pParticleEmitter = new Chimera::ParticleEmitter(posParticle, "testeZ1", 10000);
		pParticleEmitter->loadTexDiffuse("TexParticleEmmiter" , dirDados + std::string("Particle2.png"));

		//Adiciona um HUD ao Grapho
        Chimera::Group *gHud = new Chimera::Group( (Chimera::Node*)sceneMng->getRoot(), "HUD-Group");
		gHud->setShaderName("hud-default");
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


