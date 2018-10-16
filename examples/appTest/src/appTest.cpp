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

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main ( int argn, char** argv ) {
#else
int _tmain ( int argc, _TCHAR* argv[] ) {
#endif

#ifdef WIN32
        std::string dirDados = "C:\\Projetos\\ChimeraEngine\\models\\";
		std::string dirFontes = "C:\\Projetos\\ChimeraEngine\\fonts\\";
#else
        std::string dirDados = "/home/pagotto/Projetos/ChimeraEngine/models/";
		std::string dirFontes = "/home/pagotto/Projetos/ChimeraEngine/fonts/";
#endif

    using namespace Chimera;

    auto console = spdlog::stdout_color_st("main");
    console->info("appTest Iniciado");
    for (int i=0 ; i < argn; i++) {
        console->info("Parametros {0}: {1}", i, argv[i]);
    }

    try {

		std::string config_file = "./examples/appTest/etc/appteste.yaml";
        console->info("Carregar arquivo:{0}",config_file);
        YAML::Node config = YAML::LoadFile(config_file);

        YAML::Node screen = config["screen"];
        std::string nome = screen["name"].as<std::string>();

        YAML::Node canvas = screen["canvas"];
        int w = canvas["w"].as<int>();
        int h = canvas["h"].as<int>();

		YAML::Node shader = config["shader"];
		std::string vertexFile = shader["vertex"].as<std::string>();
		std::string fragmentFile = shader["fragment"].as<std::string>();

        console->info("Iniciar Tela: {0}, w: {1}, h: {2}", nome, w, h);
		console->info("Shader: Vertex: {0}", vertexFile);
		console->info("Shader: Fragment: {0}", fragmentFile);

        //Instancia de Video
        //Chimera::Video *video = new Chimera::OvrDevice("Teste");
        Video *video = new VideoDevice (w, h, nome);

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

		Shader *shader_engine =  Singleton<Shader>::getRefSingleton();
		shader_engine->load("default", vertexFile, fragmentFile);

        Group* group1 = sceneMng->createSceneGraph();
        group1->setShaderName("default");

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

        Singleton<Shader>::releaseRefSingleton();

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
