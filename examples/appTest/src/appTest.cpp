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
#include "ShadersManager.h"

#include "CameraSpherical.h"

#include "Light.h"

#include "MeshUtil.h"

#include "Group.h"

#include <glm/glm.hpp>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

    using namespace Chimera;

    auto console = spdlog::stdout_color_st("chimera");
    
    spdlog::set_level(spdlog::level::debug);

    console->info("appTest Iniciado");
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {
		std::string config_file = "./examples/appTest/etc/appteste.yaml";
        console->info("Carregar arquivo:{0}",config_file);

        YAML::Node config = YAML::LoadFile(config_file);
        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        Video *video = new VideoDevice (canvas["w"].as<int>(), canvas["h"].as<int>(), screen["name"].as<std::string>());

        //Carga dos shaders
		YAML::Node shaders = config["shaders"];
        Chimera::ShadersManager *shader_engine =  Chimera::Singleton<Chimera::ShadersManager>::getRefSingleton();
        console->info("Shaders identificados: {0}", shaders.size());
        for (std::size_t i=0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];
            shader_engine->load(shader_item["name"].as<std::string>(),
                                shader_item["vertex"].as<std::string>(),
                                shader_item["fragment"].as<std::string>());
        }


        SceneMng *sceneMng = new SceneMng ( );

        Group* group1 = new Group(sceneMng, "modelos");
        group1->setShaderName("mesh-default");
		//group1->setShadowMap(new Chimera::ShadowMapVisitor("shadow1",2048,2048));

        //define a origem da rotacao da camera!!
        Transform* pTrans = new Transform(group1,"trans01");
        pTrans->setPosition( glm::vec3( 0.0, 0.0, 0.0) );
        sceneMng->origemDesenho((Coord*)pTrans);

        CameraSpherical *pCam = new CameraSpherical("Observador-01");
        pCam->setDistanciaMaxima(1000.0);
        pCam->setDistanciaMinima(0.5);
        pCam->setFar(10000.0);
        pCam->setNear(0.1);
        pCam->setFov(45.0);
        pCam->setPositionRotation( glm::vec3(300 , 0 ,0) , glm::vec3(0,0,0) );
        pCam->setPerspective(true);
        pCam->init();
        group1->addChild(pCam);

        Light* pLight = new Light(group1,"Luz1");
        pLight->setDiffuse(Color::WHITE);
        pLight->setPositionRotation(glm::vec3(80,100,150), glm::vec3(0,0,0));
        //pLight->init();

        //Texture *pTex = new Texture("Texture-teste",dirDados + "/spacebox.png");
        //Texture *pTex = new Texture("Texture-teste", TEX_SEQ::DIFFUSE, "./models/image1.jpg");
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

        Game *game = new Game(sceneMng, video);

        FlowControl *pControle = new FlowControl( game );
        pControle->open();
        pControle->gameLoop();

        Singleton<ShadersManager >::releaseRefSingleton();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave:{0}", ex.getMessage());
        //std::cout << "Falha grave: " << ex.getMessage() << " " << std::endl;
        return -1;
    } catch (const std::exception& ex) {
        console->error("Falha grave:{0}", ex.what());
        //std::cout << "Falha grave: " << ex.what() << " " << std::endl;
    } catch (const std::string& ex) {
        console->error("Falha grave:{0}", ex);
    } catch (...) {
        console->error("Falha Desconhecida");
        //std::cout << "Falha Desconhecida " << std::endl;
    }

	console->info("appTest finalizado com sucesso");

    return 0;
}
