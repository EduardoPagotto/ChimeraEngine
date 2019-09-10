#include "Game.hpp"
//#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/core/ShadersLoader.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/MeshUtil.hpp"
#include "chimera/node/Transform.hpp"

#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

#ifndef WIN32
int main(int argn, char** argv) {
#else
int _tmain(int argc, _TCHAR* argv[]) {
#endif

    using namespace Chimera;

    std::map<std::string, Chimera::Shader*> mapa;

    Chimera::Logger* console = Chimera::Logger::get();
    // spdlog::set_level(spdlog::level::debug);

    console->info("appTest Iniciado");
    for (int i = 0; i < argn; i++) {
        console->info("Parametros %d: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./examples/appTest/etc/appteste.yaml";
        console->info("Carregar arquivo: " + config_file);

        YAML::Node config = YAML::LoadFile(config_file);
        YAML::Node screen = config["screen"];
        YAML::Node canvas = screen["canvas"];

        // Controlador de video
        CanvasHmd* video =
            new CanvasHmd(screen["name"].as<std::string>(), canvas["w"].as<int>(), canvas["h"].as<int>());
        // CanvasGL* video = new CanvasHmd(screen["name"].as<std::string>(), canvas["w"].as<int>(),
        // canvas["h"].as<int>());

        // Gerenciador do grapho de cena
        SceneMng* sceneMng = new SceneMng();

        Chimera::ShadersLoader sl;
        // Carga dos shaders
        YAML::Node shaders = config["shaders"];

        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        console->info("Shaders identificados: %d", shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];
            Chimera::Shader* pShader =
                sl.loadShader(shader_item["name"].as<std::string>(), shader_item["vertex"].as<std::string>(),
                              shader_item["fragment"].as<std::string>());

            mapa[pShader->getCurrentProgram()] = pShader;
        }

        Group* group1 = new Group(sceneMng, "modelos");
        group1->setShader(mapa["mesh-default"]);
        // group1->setShadowMap(new Chimera::ShadowMapVisitor("shadow1",2048,2048));

        // define a origem da rotacao da camera!!
        Transform* pTrans = new Transform(group1, "trans01");
        pTrans->setPosition(glm::vec3(0.0, 0.0, 0.0));
        sceneMng->origemDesenho((Coord*)pTrans);

        // Propriedades da camera
        Camera* pCam = new Camera(group1, "Observador-01");
        pCam->createTrackBall();
        pCam->createTrackHead();
        pCam->getTrackBall()->setMax(1000.0);
        pCam->getTrackBall()->setMin(0.5);
        pCam->getViewPoint()->far = 10000.0;
        pCam->getViewPoint()->near = 0.1;
        pCam->getViewPoint()->fov = 45.0;
        pCam->getViewPoint()->position = glm::vec3(0, 0, 300);
        pCam->getViewPoint()->front = glm::vec3(0, 0, 0);
        pCam->getViewPoint()->up = glm::vec3(0, 1, 0);

        // pCam->getViewPoint()->rotation = glm::vec3(0, 0, 1);
        pCam->init();

        // Propriedades da luz
        Light* pLight = new Light(group1, "Luz1");
        pLight->setDiffuse(Color::WHITE);
        pLight->setAmbient(Chimera::Color(0.2f, 0.2f, 0.2f));
        pLight->setPositionRotation(glm::vec3(80, 100, 150), glm::vec3(0, 0, 0));

        // Material do cubo 1 com textura
        Material* pMat1 = new Material("Mat1");
        pMat1->setAmbient(Color(1.0f, 0.5f, 0.31f));
        pMat1->setDiffuse(Color(1.0f, 0.5f, 0.31f));
        pMat1->setSpecular(Color(0.5f, 0.5f, 0.5f));
        pMat1->setShine(32.0f);
        pMat1->loadTextureFromFile("Texture-teste", TEX_KIND::DIFFUSE, "./models/image1.jpg");

        // Mesh do cubo1 filho de posicao 1
        Mesh* pMesh = Chimera::createMeshParallelepiped2(pTrans, "Cubo-01", glm::vec3(50, 50, 50), pMat1);

        // Posicao Cubo2
        Transform* pTrans2 = new Transform(group1, "trans02");
        pTrans2->setPosition(glm::vec3(150.0, 0.0, 0.0));

        // Material Cubo 2 sem textura
        Material* pMat2 = new Material("Mat2");
        pMat2->setAmbient(Color(0.5f, 0.5f, 0.31f));
        pMat2->setDiffuse(Color(0.5f, 0.5f, 0.5f));
        pMat2->setSpecular(Color(0.5f, 0.5f, 0.5f));
        pMat2->setShine(32.0f);

        // Mesh do cubo 2 vinculado posicao 2
        Mesh* pMesh2 = Chimera::createMeshParallelepiped2(pTrans2, "Cubo-02", glm::vec3(20, 20, 20), pMat2);

        // Wrapper do game
        Game* game = new Game(sceneMng, video);

        // Controle do fluxo de programa
        FlowControl* pControle = new FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        console->info("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete sceneMng;
        delete video;

        mapa.clear();

    } catch (const Chimera::Exception& ex) {
        console->error("Falha grave: " + ex.getMessage());
        return -1;
    } catch (const std::exception& ex) { console->error("Falha grave: %s", ex.what()); } catch (const std::string& ex) {

        console->error("Falha grave:" + ex);

    } catch (...) { console->error("Falha Desconhecida"); }

    console->info("appTest finalizado com sucesso");
    return 0;
}
