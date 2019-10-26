#include "Game.hpp"
//#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/FlowControl.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/render/CanvasHmd.hpp"
#include "chimera/render/LoadObj.hpp"
#include "chimera/render/Transform.hpp"

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
        // CanvasGL* video = new CanvasGL(screen["name"].as<std::string>(), canvas["w"].as<int>(),
        // canvas["h"].as<int>());

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

        Group* group1 = new Group(nullptr, "modelos");
        SceneMng* sceneMng = new SceneMng(video, group1);

        group1->setShader(mapa["mesh-default"]);
        // group1->setShadowMap(new Chimera::ShadowMapVisitor("shadow1",2048,2048));

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
        pCam->init();

        // Propriedades da luz
        Light* pLight = new Light(group1, "Luz1");
        pLight->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        pLight->setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
        pLight->setPosition(glm::vec3(80, 100, 150));

        // define a origem da rotacao da camera!!
        Transform* pTrans = new Transform();
        pTrans->setPosition(glm::vec3(0.0, 0.0, 0.0));
        sceneMng->origemDesenho(pTrans);

        // Mesh do cubo1 filho de posicao 1
        Material matObj = Material();
        Mesh* pMesh = Chimera::createEmpty(group1, "Cubo-01", &matObj);
        pMesh->setTransform(pTrans);
        std::string materialFile;
        loadObj("./data/models/cubo_textura_simples.obj", pMesh->meshData, materialFile);
        loadMtl(materialFile, &matObj);
        pMesh->meshData.changeSize(5.0, matObj.hasTexture());

        // Material Cubo 2 sem textura
        // Material* pMat2 = new Material();
        // pMat2->setAmbient(glm::vec4(0.5f, 0.5f, 0.31f, 1.0f));
        // pMat2->setDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        // pMat2->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        // pMat2->setShine(32.0f);
        // pMat2->addTexture(new TexImg(TEX_KIND::DIFFUSE, "./data/images/image1.jpg"));

        // Posicao Cubo2
        Transform* pTrans2 = new Transform();
        pTrans2->setPosition(glm::vec3(150.0, 0.0, 0.0));

        // Mesh do cubo 2 vinculado posicao 2
        Material matObj2 = Material();
        Mesh* pMesh2 = Chimera::createEmpty(group1, "Cubo-02", &matObj2);
        pMesh2->setTransform(pTrans2);
        loadObj((const char*)"./data/models/cubo2.obj", pMesh2->meshData, materialFile);
        loadMtl(materialFile, &matObj2);
        pMesh2->meshData.changeSize(10.0, matObj2.hasTexture());
        matObj2.setShine(32.0f);

        // Wrapper do game
        Game* game = new Game(sceneMng);

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
