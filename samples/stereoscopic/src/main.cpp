#include "Game.hpp"
//#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/CanvasHmd.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/LoadObj.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/Transform.hpp"

#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

int main(int argn, char** argv) {

    using namespace Chimera;

    std::map<std::string, Chimera::Shader*> mapa;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("appTest Iniciado");

    for (int i = 0; i < argn; i++) {
        SDL_Log("Parametros %d: %s", i, argv[i]);
    }

    try {
        std::string config_file = "./samples/stereoscopic/appteste.yaml";
        SDL_Log("Carregar arquivo: %s", config_file.c_str());

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

        // Carga dos shaders
        YAML::Node shaders = config["shaders"];

        // Chimera::ShadersManager *shader =  sceneMng->getShadersManager();
        SDL_Log("Shaders identificados: %d", (int)shaders.size());
        for (std::size_t i = 0; i < shaders.size(); i++) {
            YAML::Node shader_item = shaders[i];

            std::string nome = shader_item["name"].as<std::string>();
            std::string vertex = shader_item["vertex"].as<std::string>();
            std::string fragm = shader_item["fragment"].as<std::string>();
            Chimera::Shader* pShader =
                new Chimera::Shader(nome.c_str(), Chimera::shadeLoadProg(nome.c_str(), vertex.c_str(), fragm.c_str()));

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
        // FIXME: aqui
        Mesh* pMesh = Chimera::createEmpty(pTrans, "Cubo-01", pMat1);
        std::string materialFile;
        loadObj("./samples/bsptree/models/cube.obj", pMesh->meshData, materialFile);
        pMesh->meshData.changeSize(25.0, pMat1->hasTexture());

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
        Mesh* pMesh2 = Chimera::createEmpty(pTrans2, "Cubo-02", pMat2);
        loadObj((const char*)"./samples/bsptree/models/cube.obj", pMesh2->meshData, materialFile);
        pMesh2->meshData.changeSize(20.0, pMat2->hasTexture());

        // Wrapper do game
        Game* game = new Game(sceneMng, video);

        // Controle do fluxo de programa
        FlowControl* pControle = new FlowControl(game);
        pControle->open();
        pControle->gameLoop();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete sceneMng;
        delete video;

        mapa.clear();

        SDL_Log("appTest finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fail 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
