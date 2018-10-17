#ifndef WIN32
#include <cstdio>
#else
#include "stdafx.h"
#endif

#include <iostream>

#include <VideoDevice.h>

#ifdef OVR_SET_TO_USE
#include "OvrDevice.h"
#endif

#include <Game.h>
#include <LoaderDae.h>
#include <FlowControl.h>
#include <ParticleEmitter.h>
#include <Transform.h>
#include <HUD.h>
#include <Shader.h>
#include <Singleton.h>
#include <ExceptionChimera.h>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

    auto console = spdlog::stdout_color_st("main");
    console->info("AppShader Iniciado");
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
		std::string config_file = "./examples/AppCargaManual/etc/appcargamanual.yaml";
        console->info("Carregar arquivo:{0}",config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        std::string nome = screen["name"].as<std::string>();
        YAML::Node canvas = screen["canvas"];
        int w = canvas["w"].as<int>();
        int h = canvas["h"].as<int>();

        console->info("Iniciar Tela: {0}, w: {1}, h: {2}", nome, w, h);

        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        Chimera::Video *video = new Chimera::VideoDevice ( w, h, nome );

		YAML::Node shaders = config["shaders"];
        Chimera::Shader *shader =  Chimera::Singleton<Chimera::Shader>::getRefSingleton();

        console->info("Shaders identificados: {0}", shaders.size());

        for (std::size_t i=0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];
            std::string nameShade = shader_item["name"].as<std::string>();
            std::string vertexFile = shader_item["vertex"].as<std::string>();
            std::string fragmentFile = shader_item["fragment"].as<std::string>();
            console->info("Shader: Name: {0}", nameShade);
            console->info("Shader: Vertex: {0}", vertexFile);
            console->info("Shader: Fragment: {0}", fragmentFile);
            // FIXME: colocar log de carga no shader->loader
            shader->load(nameShade, vertexFile, fragmentFile);
        }

        std::string model = config["model"].as<std::string>();
        std::string path_tex = config["path_tex"].as<std::string>();
        std::string path_model = config["path_model"].as<std::string>();

        std::string font = config["font"].as<std::string>();

        //Carga de dados
		Chimera::LoaderDae *pLoader = new Chimera::LoaderDae( path_model, path_tex, model);
        //Chimera::Node *pRoot = pLoader->loadFile("cuboTex1.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("cuboEesfera.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("testeMaterial.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("CuboEsferaMaterial.dae");
        //Chimera::Node *pRoot = pLoader->loadFile("piso2Teste.xml");
        //Chimera::Node *pRoot = pLoader->loadFile("espacoTesteZ1.xml");
        //Chimera::Node *pRoot = pLoader->loadFile("zoltan.dae");

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
		pParticleEmitter->loadTexDiffuse("TexParticleEmmiter" , path_model + std::string("Particle2.png"));

		//Adiciona um HUD ao Grapho
        Chimera::Group *gHud = new Chimera::Group( (Chimera::Node*)sceneMng->getRoot(), "HUD-Group");
		gHud->setShaderName("hud-default");
		Chimera::HUD *pHUD = new Chimera::HUD(gHud, "HUD-Default");
		Chimera::Font *pFont = new Chimera::Font ( font, 18 ); // TODO: carregar size da fonte
		pHUD->addFont ( pFont );

        delete pLoader;
        pLoader = nullptr;

        Game *game = new Game ( sceneMng );

        Chimera::FlowControl *pControle = new Chimera::FlowControl ( game );
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete sceneMng;
        delete video;

	} catch (const Chimera::Exception& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) {
        console->error("Falha Desconhecida");
    }

	console->info("AppShader finalizado com sucesso");
	return 0;
}


